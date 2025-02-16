#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update([[maybe_unused]] float deltaTime) 
{
	for (const auto& component : m_components)
	{
		component.second->Update(deltaTime);
	}

	for (const auto& component : m_components)
	{
		component.second->PostUpdate();
	}

	// Remove destroyed components
	for (const auto& component : m_components)
	{
		if (component.second->IsDestroyed())
		{
			m_components.erase(component.first);
		}
	}
}

void dae::GameObject::FixedUpdate() { }


void dae::GameObject::Render() const
{
	if (HasComponent<TransformComponent>())
	{
		const auto& pos = GetComponent<TransformComponent>()->GetPosition();
		for (const auto& component : m_components)
		{
			component.second->Render(pos);
		}
	}
}
