#include "ChaseState.h"

#include "EnemyComponent.h"
#include "Globals.h"
#include "VulnerableState.h"
#include "GameObject.h"
#include "PatrolState.h"

void ChaseState::OnEnter(dae::GameObject& gameObject)
{
	EnemyState::OnEnter(gameObject);
	m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
	std::cout << "Entered ChaseState";
}

void ChaseState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("player_unseen"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<PatrolState>());
	}

	if (event.id == make_sdbm_hash("collision_enter"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<VulnerableState>());
	}
}

void ChaseState::Update(dae::GameObject& game_object, float deltaTime)
{
	EnemyState::Update(game_object, deltaTime);
	m_EnemyComponent->ChasePlayer();
	m_EnemyComponent->SeekPlayer();
}
