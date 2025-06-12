#include "StunnedState.h"
#include "EnemyComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "WalkingEnemyState.h"

namespace dae
{
	void StunnedState::OnEnter(dae::GameObject& game_object)
	{
		EnemyState::OnEnter(game_object);
		m_EnemyComponent = game_object.GetComponent<EnemyComponent>();
	}

	void StunnedState::OnExit(dae::GameObject& game_object)
	{
		EnemyState::OnExit(game_object);
	}

	void StunnedState::HandleInput(dae::GameObject& object, const Event& event)
	{
		if (event.id == make_sdbm_hash("stun_time_ended"))
		{
			auto* enemyComponent = object.GetComponent<EnemyComponent>();
			enemyComponent->SetState(std::make_unique<WalkingEnemyState>());
		}
	}

}
