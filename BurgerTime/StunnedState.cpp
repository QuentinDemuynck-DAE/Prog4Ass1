#include "StunnedState.h"

#include "ClimbingEnemyState.h"
#include "EnemyComponent.h"
#include "Globals.h"
#include "GameObject.h"
#include "Subject.h"
#include "WalkingEnemyState.h"

namespace dae
{
	StunnedState::StunnedState(bool wasWalking)
		:m_WasWalking(wasWalking)
	{
	}

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
			if (m_WasWalking)
			{
				auto* enemyComponent = object.GetComponent<EnemyComponent>();
				enemyComponent->SetState(std::make_unique<WalkingEnemyState>());
				return;
			}

			auto* enemyComponent = object.GetComponent<EnemyComponent>();
			enemyComponent->SetState(std::make_unique<ClimbingEnemyState>());
		}

	}

	void StunnedState::Update(GameObject& gameObject, float deltaTime)
	{
		m_Timer += deltaTime;

		if (m_Timer > MAX_STUN_DURATION)
		{
			Event event = Event(make_sdbm_hash("stun_time_ended"));
			gameObject.GetSubject()->Notify(event);
		}
	}
}
