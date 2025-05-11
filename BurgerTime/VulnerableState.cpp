#include "VulnerableState.h"

#include "EnemyComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "PatrolState.h"

void VulnerableState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("stopped_spraying"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<PatrolState>());
	}
}
