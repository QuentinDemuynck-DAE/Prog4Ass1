#include "PlayerComponent.h"

#include "GameObject.h"

PlayerComponent::PlayerComponent(dae::GameObject& owner)
	:Component(owner)
{
}

void PlayerComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);
	if (m_IsShooting)
	{
		m_CurrentlyShooting += deltaTime;
		if (m_CurrentlyShooting >= MAX_PEPPER_TIME)
		{
			StopShooting();
		}
	}
	else
	{
		m_timeSinceLastPepper += deltaTime;
	}
}

void PlayerComponent::StopShooting()
{
	m_IsShooting = false;
	m_CurrentlyShooting = 0.0f;
	m_timeSinceLastPepper = 0.0f;
	m_Pepper->SetParent(nullptr);
}
