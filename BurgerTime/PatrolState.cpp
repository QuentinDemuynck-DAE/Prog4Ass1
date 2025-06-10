#include "PatrolState.h"

#include "ChaseState.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "VulnerableState.h"

namespace dae
{
	void PatrolState::OnEnter(dae::GameObject& gameObject)
	{
		EnemyState::OnEnter(gameObject);
		std::cout << "Entered patrol state";
		if (m_EnemyComponent == nullptr && gameObject.HasComponent<EnemyComponent>())
		{
			m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
		}
	}

	void PatrolState::OnExit(dae::GameObject& game_object)
	{
		EnemyState::OnExit(game_object);
		m_EnemyComponent->ResetPatrolTimer();
	}

	void PatrolState::HandleInput(dae::GameObject& object, const Event& event)
	{
		if (event.id == make_sdbm_hash("player_seen"))
		{
			auto* enemyComponent = object.GetComponent<EnemyComponent>();
			enemyComponent->SetState(std::make_unique<ChaseState>());
		}

		if (event.id == make_sdbm_hash("collision_enter"))
		{
			auto* enemyComponent = object.GetComponent<EnemyComponent>();
			enemyComponent->SetState(std::make_unique<VulnerableState>());
		}
	}

	void PatrolState::Update(dae::GameObject& gameObject, float deltaTime)
	{
		EnemyState::Update(gameObject, deltaTime);
		if (m_EnemyComponent)
		{
			m_EnemyComponent->Patrol(deltaTime);
			m_EnemyComponent->SeekPlayer();
		}
	}
}

