#include "StunnedState.h"

#include "ChaseState.h"
#include "EnemyComponent.h"
#include "Globals.h"
#include "GameObject.h"


void StunnedState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("stun_time_ended"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<ChaseState>());
	}
}
