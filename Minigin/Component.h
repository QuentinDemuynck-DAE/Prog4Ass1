#pragma once
#include <glm.hpp>
class Component
{
public:
	Component() = default;
	virtual ~Component() = default;
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Update([[maybe_unused]]float deltaTime) {};
	virtual void FixedUpdate() {};
	virtual void Render([[maybe_unused]]glm::vec3 position) const {};
	virtual void PostUpdate() {};
	void Destroy() { m_destroyed = true; }
	bool IsDestroyed() const { return m_destroyed; }
private:
	bool m_destroyed = false;
};

