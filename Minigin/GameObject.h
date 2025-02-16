#pragma once
#include <memory>
#include "Transform.h"
#include <unordered_map>
#include <typeindex>
#include "Component.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject 
	{
	public:
		virtual void Update(float deltaTime);
		virtual void FixedUpdate();
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename ComponentType>
		ComponentType* GetComponent() const
		{
			const auto id{ std::type_index(typeid(ComponentType)) };

			if (m_components.find(id) == m_components.end())
				return nullptr;

			return static_cast<ComponentType*>(m_components.at(id).get());
		}

		template <typename ComponentType>
		void AddComponent(std::unique_ptr<ComponentType> component)
		{
			const auto id{ std::type_index(typeid(ComponentType)) };
			if (m_components.find(id) != m_components.end())
				return;
			m_components[id] = std::move(component);
		}

		template <typename ComponentType>
		void RemoveComponent()
		{
			const auto id{ std::type_index(typeid(ComponentType)) };
			if (m_components.find(id) == m_components.end())
				return;
			m_components.at(id).get()->Destroy();
		}

		template <typename ComponentType>
		bool HasComponent() const
		{
			const auto id{ std::type_index(typeid(ComponentType)) };

			return (m_components.find(id) != m_components.end())
		}

	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components{};
	};
}
