#include "EnemyComponent.h"

#include <random>

#include "GameObject.h"
#include "Subject.h"
#include "Globals.h"
#include "SceneManager.h"

namespace dae
{
	EnemyComponent::EnemyComponent(dae::GameObject& owner, std::vector<dae::GameObject*> players, int value)
		:Component(owner), m_Players(players), m_ChasingPlayer(nullptr), m_PatrolDirectionTimer(0.0f), m_Value(value)
	{
	}

	void EnemyComponent::Update(float deltaTime)
	{
		if (m_CurrentState) m_CurrentState->Update(GetOwner(), deltaTime);
	}

	void EnemyComponent::HandleInput(const Event event)
	{
		if (m_CurrentState) m_CurrentState->HandleInput(GetOwner(), event);
	}

	void EnemyComponent::SetState(std::unique_ptr<EnemyState> newState)
	{
		if (m_CurrentState) m_CurrentState->OnExit(GetOwner());
		m_CurrentState = std::move(newState);
		if (m_CurrentState) m_CurrentState->OnEnter(GetOwner());
	}


	

	std::vector<dae::GameObject*> EnemyComponent::GetPlayers() const
	{
		return m_Players;
	}

	GameObject* EnemyComponent::GetClosestPlayer() const
	{
		dae::GameObject* closestPlayer = nullptr;
		float distance = FLT_MAX;

		for (dae::GameObject* player : m_Players)
		{
			if (!SceneManager::GetInstance().IsInActiveScene(player))
				continue;

			float newDistance = glm::distance(GetOwner().GetTransform()->GetGlobalPosition(), player->GetTransform()->GetGlobalPosition());
			if (newDistance < distance)
			{
				closestPlayer = player;
				distance = newDistance;
			}
		}

		return closestPlayer;
	}

	bool EnemyComponent::CanHit() const
	{
		return m_CanHit;
	}

	void EnemyComponent::SetCanHit(const bool& canHit)
	{
		m_CanHit = canHit;
	}

	const int& EnemyComponent::GetValue() const
	{
		return m_Value;
	}

	void EnemyComponent::SetPlayers(std::vector<dae::GameObject*> players)
	{
		m_Players = players;
	}
}

