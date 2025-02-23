#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Component.h"
#include "Transform.h"

namespace dae
{
	class Transform;
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void FixedUpdate();
		void PostUpdate(float deltaTime);
		void Render() const;

		GameObject(const Transform& transform, GameObject* parent = nullptr);
		GameObject(const glm::vec3 localPosition = { 0,0,0 }, const glm::vec3 localRotation = {0,0,0}, const glm::vec3 localScale = { 0,0,0 }, GameObject* parent = nullptr);

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

		template <typename ComponentType, typename... Args>
		void AddComponent(Args&&... args)
		{
			const auto id{ std::type_index(typeid(ComponentType)) };
			if (m_components.find(id) != m_components.end())
				return;

			m_components[id] = std::make_unique<ComponentType>(*this, std::forward<Args>(args)...);
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

			return (m_components.find(id) != m_components.end());
		}

		// Parenting
		GameObject* GetParent() { return m_parent; }
		void SetParent(GameObject* parent, bool keepWorldPosition = false);

		int GetChildCount() { return int(m_children.size()); }
		GameObject* GetChildAt(int index) { return m_children[index]; }

		// Transform
		Transform* GetTransform() { return m_transform.get(); }


	private:
		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components{};

		std::unique_ptr<Transform> m_transform{};

		GameObject* m_parent{nullptr};
		std::vector<GameObject*> m_children{};

		bool IsDescendant(GameObject* target);
		bool IsChildOf(GameObject* target);
		

	};
}
