#include "EnemyComponent.h"

#include <random>

#include "GameObject.h"
#include "Subject.h"
#include "Globals.h"
#include "PatrolState.h"

namespace dae
{
	EnemyComponent::EnemyComponent(dae::GameObject& owner, std::vector<dae::GameObject*> players)
		:Component(owner), m_Players(players), m_ChasingPlayer(nullptr), m_PatrolDirectionTimer(0.0f)
	{
	}

	void EnemyComponent::Update(float deltaTime)
	{
		if (m_CurrentState) m_CurrentState->Update(GetOwner(), deltaTime);
		Move(deltaTime);
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

	void EnemyComponent::SeekPlayer()
	{
		//Temp function to check the states will be changed in later version
		dae::GameObject* closestPlayer = nullptr;
		float distance = SEEK_DISTANCE;


		for (dae::GameObject* player : m_Players)
		{
			float newDistance = glm::distance(GetOwner().GetTransform()->GetGlobalPosition(), player->GetTransform()->GetGlobalPosition());
			if (newDistance < distance)
			{
				closestPlayer = player;
				distance = newDistance;
			}

			if (closestPlayer != nullptr)
			{
				Event event(make_sdbm_hash("player_seen"));
				event.AddArg(closestPlayer);
				GetOwner().GetSubject()->Notify(event);
			}

			if (closestPlayer == nullptr && m_ChasingPlayer != nullptr)
			{
				Event event(make_sdbm_hash("player_unseen"));
				GetOwner().GetSubject()->Notify(event);
			}
			m_ChasingPlayer = closestPlayer;
		}
	}

	void EnemyComponent::ChasePlayer()
	{
		glm::vec3 from = GetOwner().GetTransform()->GetGlobalPosition();
		glm::vec3 to = m_ChasingPlayer->GetTransform()->GetGlobalPosition();

		// Build the direction vector and normalize it:
		glm::vec3 direction = to - from;
		if (glm::length(direction) > 0.0f)  // avoid division by zero
			m_Direction = glm::normalize(direction);
	}

	void EnemyComponent::SetCanMove(bool canMove)
	{
		m_CanMove = canMove;
	}

	void EnemyComponent::Patrol(float dt)
	{
		if (m_PatrolDirectionTimer >= CALCULATE_NEW_PATROL_DIR)
		{
			std::mt19937_64 rng{ std::random_device{}() };
			std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

			glm::vec3 r;
			do {
				r = glm::vec3{ dist(rng), dist(rng), dist(rng) };
			} while (glm::length(r) < 0.0001f);  // reject near-zero

			m_Direction = glm::normalize(r);
			m_PatrolDirectionTimer -= CALCULATE_NEW_PATROL_DIR;
		}
		m_PatrolDirectionTimer += dt;

	}

	void EnemyComponent::ResetPatrolTimer()
	{
		m_PatrolDirectionTimer = 0.0f;
	}

	void EnemyComponent::Move(float deltaTime)
	{
		if (m_CanMove)
			GetOwner().GetTransform()->SetLocalPosition(GetOwner().GetTransform()->GetLocalPosition() + m_Direction * MOVEMENT_SPEED * deltaTime);
	}
}

