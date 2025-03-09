#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;
}

class Component
{
public:
	Component(dae::GameObject& owner);
	virtual ~Component() = default;
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update(float deltaTime);
	virtual void FixedUpdate() {};
	virtual void Render(glm::vec3 position);
	virtual void PostUpdate(float deltaTime);

	void Destroy() { m_destroyed = true; }
	bool IsDestroyed() const { return m_destroyed; }

	dae::GameObject& GetOwner() const { return m_owner; }

private:
	dae::GameObject& m_owner;
	bool m_destroyed = false;
};

