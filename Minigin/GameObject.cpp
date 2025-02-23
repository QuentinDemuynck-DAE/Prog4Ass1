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
	for (const auto& component : m_components)
	{
			component.second->Render(m_transform.get()->GetGlobalPosition());
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
		return;
	}

	if (m_parent == parent)
	{
		return; 
	}

	if (IsDescendant(parent))
	{
		return;
	}

	for (const auto& child : m_children)
	{
		if (child->GetParent() == parent)
		{
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
			m_transform.get()->SetLocalPosition(m_transform.get()->GetGlobalPosition() - parent->m_transform.get()->GetGlobalPosition());
		}
		m_transform.get()->SetPositionDirty();
	}

	// Remove from current parent if needed
	if (m_parent)
	{
		if (m_parent)
		{
			auto it = std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this);
			m_parent->m_children.erase(it, m_parent->m_children.end());
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
	if (!target) return false;

	GameObject* current = this;
	while (current)
	{
		if (current == target) return true;  // Prevent cycles
		current = current->m_parent;
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
