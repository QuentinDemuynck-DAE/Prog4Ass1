#include "PatrolState.h"

#include "ChaseState.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "VulnerableState.h"

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
