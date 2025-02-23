#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime) 
{
	for (const auto& component : m_components)
	{
		component.second->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate() 
{ 
	for (const auto& component : m_components)
	{
		component.second->FixedUpdate();
	}
}

void dae::GameObject::PostUpdate(float deltaTime)
{
	for (const auto& component : m_components)
	{
		component.second->PostUpdate(deltaTime);
	}

	std::erase_if(m_components, [](const auto& component) 
	{
		return component.second->IsDestroyed();
	});
}


void dae::GameObject::Render() const
{
	const auto& pos = m_transform.get()->GetGlobalPosition();
	for (const auto& component : m_components)
	{
			component.second->Render(pos);
	}
}

dae::GameObject::GameObject(const Transform& transform, GameObject* parent)
{
	m_transform = std::make_unique<Transform>(transform);
	SetParent(parent, false);
}

dae::GameObject::GameObject(const glm::vec3 localPosition, const glm::vec3 localRotation, const glm::vec3 localScale, GameObject* parent)
{
	m_transform = std::make_unique<Transform>(*this, localPosition, localRotation, localScale);
	SetParent(parent, false);
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if(parent == this)
	{
		assert(false && "SetParent should not be called with itself as parent");
		return;
	}


	if (IsDescendant(parent))
	{
		assert(false && "SetParent should not be called with a descendant as parent");
		return;
	}

	for (const auto& child : m_children)
	{
		if (child->GetParent() == parent)
		{
			assert(false && "SetParent should not be called with a child as parent");
			return;
		}
	}

	if (parent == nullptr)
	{
		m_transform.get()->SetLocalPosition(m_transform.get()->GetGlobalPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			m_transform.get()->SetLocalPosition(m_transform.get()->GetGlobalPosition() - m_parent->m_transform.get()->GetGlobalPosition());
		}
		m_transform.get()->SetPositionDirty();
	}

	// Remove from current parent if needed
	if (m_parent)
	{
		auto& siblings = m_parent->m_children;
		auto it = std::find_if(siblings.begin(), siblings.end(),
			[this](const GameObject* child) { return child == this; });

		if (it != siblings.end())
		{
			siblings.erase(it); // Erase this object from the parent's children list
		}
	}

	// Assign new parent
	m_parent = parent;

	// Add this object to the new parent's children list (if a new parent exists)
	if (m_parent)
	{
		m_parent->m_children.push_back(this); // Transfer ownership
	}
}


bool dae::GameObject::IsDescendant(GameObject* target)
{
	for (const auto& child : m_children)
	{
		if (child == target || child->IsDescendant(target))
		{
			return true;
		}
	}
	return false;
}

bool dae::GameObject::IsChildOf(GameObject* target)
{
	GameObject* current = this;
	while (current)
	{
		if (current == target)
		{
			return true;
		}
		current = current->m_parent; // Move up the hierarchy
	}
	return false;
}
