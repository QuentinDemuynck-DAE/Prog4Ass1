#include "EnemyDying.h"

#include "GameManager.h"
#include "GameObject.h"
#include "Subject.h"
#include "WalkingEnemyState.h"

void dae::EnemyDying::HandleInput(GameObject& gameObject, const Event& event)
{

	if (event.id == make_sdbm_hash("death_time_ended"))
	{
		if (event.numberArgs >= 1
			&& std::holds_alternative<void*>(event.args[0]))
		{

			//Only check if we sent it
			auto sender = static_cast<GameObject*>(
				std::get<void*>(event.args[0]));

			if (&gameObject == sender && m_EnemyComponent)
			{
				auto state = std::make_unique<WalkingEnemyState>();
				m_EnemyComponent->SetState(std::move(state));
			}
		}
	}
}

void dae::EnemyDying::OnEnter(GameObject& gameObject)
{
	if (gameObject.HasComponent<EnemyComponent>())
	{
		m_EnemyComponent = gameObject.GetComponent<EnemyComponent>();
		m_EnemyComponent->SetCanHit(false);
	}

	if (gameObject.HasComponent<MapWalkerComponent>())
	{
		m_MapWalker = gameObject.GetComponent<MapWalkerComponent>();
	}
}

void dae::EnemyDying::OnExit(GameObject& )
{
	if (m_EnemyComponent)
	{
		m_EnemyComponent->SetCanHit(true);
		GameManager::GetInstance().AddScore(m_EnemyComponent->GetValue());
	}

	if (m_MapWalker)
	{
		m_MapWalker->Respawn();
	}
}

void dae::EnemyDying::Update(GameObject& gameObject, float dt)
{
	m_Timer += dt;
	if (m_Timer > RESPAWN_TIME)
	{
		Event event = Event(make_sdbm_hash("death_time_ended"));
		event.AddArg(&gameObject);
		gameObject.GetSubject()->Notify(event);
	}

}
