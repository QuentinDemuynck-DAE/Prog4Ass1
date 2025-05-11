#include "ChaseState.h"

#include "EnemyComponent.h"
#include "Globals.h"
#include "VulnerableState.h"
#include "GameObject.h"
#include "PatrolState.h"

void ChaseState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("player_unseen"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<PatrolState>());
	}

	if (event.id == make_sdbm_hash("pepper_hit"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<VulnerableState>());
	}
}
