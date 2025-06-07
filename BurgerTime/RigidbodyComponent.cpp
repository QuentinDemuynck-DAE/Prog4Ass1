#include "RigidbodyComponent.h"
#include "GameObject.h"

RigidbodyComponent::RigidbodyComponent(dae::GameObject& owner, float maxSpeed, float mass, float drag)
	: Component(owner)
	, m_Mass{ mass }
	, m_Drag{ drag }
	, m_MaxSpeed{ maxSpeed }
{
}

void RigidbodyComponent::Update(float deltaTime)
{
	auto& owner = GetOwner();

	// Will make this better in the future
	if (glm::length(m_Velocity) >= m_MaxSpeed)
	{
		auto normalVel = normalize(m_Velocity);
		m_Velocity = m_MaxSpeed * normalVel;
	}

	// Apply drag
	m_Velocity.x -= m_Velocity.x * m_Drag;
	m_Velocity.y -= m_Velocity.y * m_Drag;

	owner.GetTransform()->SetLocalPosition(owner.GetTransform()->GetLocalPosition() + glm::vec3(m_Velocity.x * deltaTime, m_Velocity.y * deltaTime, 0));

}

void RigidbodyComponent::AddVelocity(float x, float y)
{
	m_Velocity.x += x;
	m_Velocity.y += y;
}

void RigidbodyComponent::AddVelocity(glm::vec2 velocity)
{
	m_Velocity += velocity;
}

void RigidbodyComponent::SetDrag(float drag)
{
	m_Drag = drag;
}

glm::vec2 RigidbodyComponent::GetVelocity() const
{
	return m_Velocity;
}

float RigidbodyComponent::GetDrag() const
{
	return m_Drag;
}
