#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Command.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Subject.h"

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
		component.second->Render(
			m_transform->GetGlobalPosition(),
			glm::vec2(m_transform->GetGlobalScale())
		);
	}
}

dae::GameObject::GameObject(const Transform& transform, GameObject* parent)
{
	m_transform = std::make_unique<Transform>(transform);
	SetParent(parent, false);
	m_pSubject = std::make_unique<Subject>(this);
}

dae::GameObject::GameObject(const glm::vec3 localPosition, const glm::vec3 localRotation, const glm::vec3 localScale, GameObject* parent)
{
	m_transform = std::make_unique<Transform>(*this, localPosition, localRotation, localScale);
	SetParent(parent, false);
	m_pSubject = std::make_unique<Subject>(this);

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
		m_transform.get()->SetLocalScale(m_transform.get()->GetGlobalScale());

	}
	else
	{
		if (keepWorldPosition)
		{
			m_transform.get()->SetLocalPosition(m_transform.get()->GetGlobalPosition() - parent->m_transform.get()->GetGlobalPosition());
			m_transform.get()->SetLocalScale(m_transform.get()->GetGlobalScale() - parent->m_transform.get()->GetGlobalScale());
		}
		m_transform.get()->SetPositionDirty();
		m_transform.get()->SetScaleDirty();
	}

	// Remove from current parent if needed
	if (m_parent)
	{
		if (m_parent)
{
	auto it = std::remove_if(
		m_parent->m_children.begin(),
		m_parent->m_children.end(),
		[this](const std::shared_ptr<GameObject>& child)
		{
			return child.get() == this;
		});
	m_parent->m_children.erase(it, m_parent->m_children.end());
}


	}

	// Assign new parent
	m_parent = parent;

	// Add this object to the new parent's children list (if a new parent exists)
	if (m_parent)
	{
		m_parent->m_children.push_back(shared_from_this()); // Transfer ownership
	}
}


dae::Subject* dae::GameObject::GetSubject()
{
	return m_pSubject.get();
}

void dae::GameObject::MarkDestroy()
{
	m_MarkedForDestroy = true;
}

void dae::GameObject::Destroy()
{
	for (auto& childPtr : m_children)
	{
		childPtr->Destroy();
	}
	m_children.clear();

	for (auto& kv : m_components)
	{
		kv.second->Destroy();
	}
	m_components.clear();

	m_pBoundCommands.clear();

	m_pSubject.reset();

	if (m_parent)
	{
		auto& siblings = m_parent->m_children;
		siblings.erase(
			std::remove_if(
				siblings.begin(),
				siblings.end(),
				[this](const std::shared_ptr<GameObject>& ptr) {
					return ptr.get() == this;
				}),
			siblings.end());
		SetParent(nullptr);
	}
}

void dae::GameObject::SetVisibility(bool visible)
{
	m_Visible = visible;
}

bool dae::GameObject::GetVisibility()
{
	return m_Visible;
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
