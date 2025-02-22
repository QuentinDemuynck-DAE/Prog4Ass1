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
	if (HasComponent<TransformComponent>())
	{
		const auto& pos = GetComponent<TransformComponent>()->GetPosition();
		for (const auto& component : m_components)
		{
			component.second->Render(pos);
		}
	}
}
