#include "PlayerComponent.h"

#include "Events.h"
#include "GamePad.h"
#include "GameObject.h"
#include "PlayerState.h"
#include "RigidbodyComponent.h"
#include "Walking.h"

PlayerComponent::PlayerComponent(dae::GameObject& owner)
	:Component(owner)
{
	if (owner.HasComponent<dae::RigidbodyComponent>())
		m_Rigidbody = owner.GetComponent<dae::RigidbodyComponent>();
}

void PlayerComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	m_TimeSpawned += deltaTime;
	if (m_IsShooting)
	{
		m_CurrentlyShooting += deltaTime;
		if (m_CurrentlyShooting >= PEPPER_COOLDOWN)
		{
			StopShooting();
		}
	}
	else
	{
		m_timeSinceLastPepper += deltaTime;
	}

	if (m_CurrentState) m_CurrentState->Update(GetOwner(), deltaTime);
}


void PlayerComponent::HandleInput(const Event event)
{
	if (m_CurrentState) m_CurrentState->HandleInput(GetOwner(), event);
}

void PlayerComponent::SetState(std::unique_ptr<dae::PlayerState> newState)
{
	if (m_CurrentState) m_CurrentState->OnExit(GetOwner());
	m_CurrentState = std::move(newState);
	if (m_CurrentState) m_CurrentState->OnEnter(GetOwner());
}

void PlayerComponent::StopShooting()
{
	m_IsShooting = false;
	m_CurrentlyShooting = 0.0f;
	m_timeSinceLastPepper = 0.0f;
	m_Rigidbody->Resume();
	m_Pepper->MarkDestroy();
}

void PlayerComponent::SetCanShoot(const bool& canShoot)
{
	m_CanShoot = canShoot;
}

bool PlayerComponent::IsInvincible() const
{
	return (m_TimeSpawned < INVINCIBLE_TIME);
}

void PlayerComponent::StartInvincibleCycle()
{
	m_TimeSpawned = 0.0f;
}
