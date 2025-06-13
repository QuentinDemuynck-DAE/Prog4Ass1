#include "RigidbodyComponent.h"
#include "GameObject.h"

dae::RigidbodyComponent::RigidbodyComponent(dae::GameObject& owner, float maxSpeed, float mass, float drag)
	: Component(owner)
	, m_Mass{ mass }
	, m_Drag{ drag }
	, m_MaxSpeed{ maxSpeed }
{
}

void dae::RigidbodyComponent::Update(float deltaTime)
{
	auto& owner = GetOwner();

	if (m_Freezed)
		return;

	// Will make this better in the future
	if (glm::length(m_Velocity) >= m_MaxSpeed)
	{
		auto normalVel = normalize(m_Velocity);
		m_Velocity = m_MaxSpeed * normalVel;
	}


	owner.GetTransform()->SetLocalPosition(owner.GetTransform()->GetLocalPosition() + glm::vec3(m_Velocity.x * deltaTime ,m_Velocity.y * deltaTime, 0));


	// Apply drag
	m_Velocity.x -= m_Velocity.x * m_Drag;
	m_Velocity.y -= m_Velocity.y * m_Drag;
}

void dae::RigidbodyComponent::AddVelocity(float x, float y)
{
	if (m_Freezed)
		return;

	m_Velocity.x += x;
	m_Velocity.y += y;
	m_LastAddedVelocity = glm::vec2{x, y};

}

void dae::RigidbodyComponent::AddVelocity(glm::vec2 velocity)
{
	if (m_Freezed)
		return;

	m_Velocity += velocity;
	m_LastAddedVelocity = velocity;
}

void dae::RigidbodyComponent::SetDrag(float drag)
{
	m_Drag = drag;
}

glm::vec2 dae::RigidbodyComponent::GetVelocity() const
{
	return m_Velocity;
}

float dae::RigidbodyComponent::GetDrag() const
{
	return m_Drag;
}

void dae::RigidbodyComponent::Stop()
{
	m_Velocity = { 0,0 };
}

void dae::RigidbodyComponent::Freeze()
{
	m_Freezed = true;
}

void dae::RigidbodyComponent::Resume()
{
	m_Freezed = false;
}

glm::vec2 dae::RigidbodyComponent::GetLastAddedVelocity() const
{
	return m_LastAddedVelocity;
}
