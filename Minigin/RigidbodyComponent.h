#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae 
{
	class GameObject; 
}
class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent(dae::GameObject& owner, float maxSpeed = 10.f, float mass = 10.f, float drag = 0.0f);
	~RigidbodyComponent() = default;
	void Update(float deltaTime) override;
	void AddVelocity(float x, float y);
	void AddVelocity(glm::vec2 velocity);
	void SetDrag(float drag);
	glm::vec2 GetVelocity() const;
	float GetDrag() const;
private:
	float m_Mass;
	float m_Drag;
	glm::vec2 m_Velocity{0.f,0.f};
	float m_MaxSpeed{ 10.f };
};

