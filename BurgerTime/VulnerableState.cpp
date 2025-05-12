#include "VulnerableState.h"

#include "EnemyComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "PatrolState.h"

void VulnerableState::OnEnter(dae::GameObject& game_object)
{
	EnemyState::OnEnter(game_object);
	m_EnemyComponent = game_object.GetComponent<EnemyComponent>();
	m_EnemyComponent->SetCanMove(false);
}

void VulnerableState::HandleInput(dae::GameObject& object, const Event& event)
{
	if (event.id == make_sdbm_hash("stopped_spraying"))
	{
		auto* enemyComponent = object.GetComponent<EnemyComponent>();
		enemyComponent->SetState(std::make_unique<PatrolState>());
	}
}
