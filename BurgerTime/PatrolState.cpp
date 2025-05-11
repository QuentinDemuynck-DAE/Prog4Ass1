#include "PatrolState.h"

#include "ChaseState.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "VulnerableState.h"

void PatrolState::OnEnter(dae::GameObject& gameObject)
{
	EnemyState::OnEnter(gameObject);
	std::cout << "Entered patrol state";
	if (m_EnemyComponent == nullptr)
	{
		m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
	}
}

void PatrolState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("player_seen"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<ChaseState>());
	}

	if (event.id == make_sdbm_hash("pepper_hit"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<VulnerableState>());
	}
}

void PatrolState::Update(dae::GameObject& gameObject, float deltaTime)
{
	EnemyState::Update(gameObject, deltaTime);
	gameObject.GetComponent<EnemyComponent>()->SeekPlayer();
}
