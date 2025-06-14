#include "Dying.h"

#include "GameManager.h"
#include "Globals.h"
#include "PlayerComponent.h"
#include "Walking.h"
#include "LivesComponent.h"
#include "Subject.h"

void dae::Dying::OnEnter(dae::GameObject& gameObject)
{
	if (gameObject.HasComponent<LivesComponent>())
	{
		m_LivesComponent = gameObject.GetComponent<LivesComponent>();
	}

	if (gameObject.HasComponent<PlayerComponent>())
	{
		m_PlayerComponent = gameObject.GetComponent<PlayerComponent>();
	}

	if (gameObject.HasComponent<MapWalkerComponent>())
	{
		m_MapWalkerCommponent = gameObject.GetComponent<MapWalkerComponent>();
	}
}

void dae::Dying::HandleInput(dae::GameObject& gameObject, const Event& event)
{
	if (event.id == make_sdbm_hash("death_time_ended"))
	{
		if (event.numberArgs >= 1
			&& std::holds_alternative<void*>(event.args[0]))
		{

			//Only check if we sent it
			auto sender = static_cast<GameObject*>(
				std::get<void*>(event.args[0]));

			if (&gameObject == sender && m_PlayerComponent)
			{
				auto state = std::make_unique<Walking>();
				m_PlayerComponent->SetState(std::move(state));
			}
		}
	}
}

void dae::Dying::Update(dae::GameObject& gameObject, float deltaTime)
{
	m_Timer += deltaTime;

	if (m_Timer > RESPAWN_TIME)
	{
		Event event = Event(make_sdbm_hash("death_time_ended"));
		event.AddArg(&gameObject);
		gameObject.GetSubject()->Notify(event);
	}
}

void dae::Dying::OnExit(GameObject& )
{
	if (!m_LivesComponent || !m_MapWalkerCommponent)
		return;
	m_LivesComponent->LoseLive();
	m_MapWalkerCommponent->Respawn();
	m_PlayerComponent->StartInvincibleCycle();
}
