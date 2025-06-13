#include "StunnedState.h"

#include "ClimbingEnemyState.h"
#include "EnemyComponent.h"
#include "EnemyDying.h"
#include "EnemyFallingAlong.h"
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
		m_EnemyComponent->SetCanHit(false);

		if (game_object.HasComponent<CollisionComponent>())
		{
			m_CollisionComponent = game_object.GetComponent<CollisionComponent>();
			
		}
	}

	void StunnedState::OnExit(dae::GameObject&)
	{
		if (m_EnemyComponent)
		{
			m_EnemyComponent->SetCanHit(true);
		}
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


		if (event.id == make_sdbm_hash("ingredient_started_falling"))
		{
			if (event.numberArgs >= 1
				&& std::holds_alternative<void*>(event.args[0]))
			{
				auto sender = static_cast<GameObject*>(
					std::get<void*>(event.args[0]));

				auto state = std::make_unique<EnemyFallingAlong>(sender);
				m_EnemyComponent->SetState(std::move(state));
			}
		}

		if (event.id == make_sdbm_hash("ingredient_fell_on_enemy"))
		{
			if (event.numberArgs >= 1
				&& std::holds_alternative<void*>(event.args[0]))
			{

				auto state = std::make_unique<EnemyDying>();
				m_EnemyComponent->SetState(std::move(state));
			}
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
