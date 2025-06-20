#include "FallingState.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "FallingOnPlate.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include "MapTileComponent.h"
#include "RigidbodyComponent.h"
#include "IngredientComponent.h"
#include "LandedState.h"
#include "Subject.h"

void dae::FallingState::HandleInput(dae::GameObject& gameObject, const Event& event)
{
	// COLLISION ENTER
	if (event.id == make_sdbm_hash("collision_enter"))
	{
		// 0 = sender 1= other;
		if (event.numberArgs >= 2
			&& std::holds_alternative<void*>(event.args[0])
			&& std::holds_alternative<void*>(event.args[1]))
		{

			//Only check if we sent it
			auto sender = static_cast<CollisionComponent*>(
				std::get<void*>(event.args[0]));

			auto receiver = static_cast<CollisionComponent*>(
				std::get<void*>(event.args[1]));

			if (!(sender && receiver && sender == gameObject.GetComponent<CollisionComponent>()))
				return;

			auto& gob = receiver->GetOwner();

			if (gob.HasComponent<MapTileComponent>() && m_pIngredientComponent) 
			{
				auto tile = gob.GetComponent<MapTileComponent>();

				if (tile->GetFloor() && m_Timer > CHECK_TIME)
				{
					m_Timer = 0;
					--m_LayersToFall;
				}

				if (tile->GetPlate())
				{
					auto state = std::make_unique<FallingOnPlate>();
					m_pIngredientComponent->SetState(std::move(state));
				}
			}
			else if (gob.HasComponent<EnemyComponent>())
			{
				Event eventNew = Event(make_sdbm_hash("ingredient_fell_on_enemy"));
				eventNew.AddArg(&gameObject);
				gob.GetSubject()->Notify(eventNew);
			}
		}
	}

	if (m_LayersToFall == 0)
	{
		auto state = std::make_unique<LandedState>();
		m_pIngredientComponent->SetState(std::move(state));
	}
}

void dae::FallingState::OnEnter(dae::GameObject& gameObject)
{
	if (gameObject.HasComponent<RigidbodyComponent>())
	{
		m_Rigidbody = gameObject.GetComponent<RigidbodyComponent>();
	}

	if (gameObject.HasComponent<IngredientComponent>())
	{
		m_pIngredientComponent = gameObject.GetComponent<IngredientComponent>();
		m_LayersToFall += m_pIngredientComponent->NumberOfEnemiesStanding();
		m_NumEnemiesInitial = m_pIngredientComponent->NumberOfEnemiesStanding();
	}
}

void dae::FallingState::OnExit(dae::GameObject& gameObject)
{
	if (m_Rigidbody)
	{
		m_Rigidbody->Stop();

	}

	GameManager::GetInstance().AddScore(GetIngredientsTarget(m_NumEnemiesInitial));


	if (m_pIngredientComponent)
	{
		Event event = Event(make_sdbm_hash("ingredient_landed"));
		event.AddArg(&gameObject);

		for (auto& enemy : m_pIngredientComponent->GetEnemiesStanding())
		{
			enemy->GetSubject()->Notify(event);
		}
	}
}

void dae::FallingState::Update(dae::GameObject&, float dt)
{
	m_Timer += dt;

	if (m_Rigidbody)
	{
		m_Rigidbody->AddVelocity(m_Speed);
	}
}
