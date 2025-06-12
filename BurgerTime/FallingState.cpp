#include "FallingState.h"

#include "CollisionComponent.h"
#include "FallingOnPlate.h"
#include "GameObject.h"
#include "Globals.h"
#include "MapTileComponent.h"
#include "RigidbodyComponent.h"
#include "IngredientComponent.h"
#include "LandedState.h"

void dae::FallingState::HandleInput(dae::GameObject& gameObject, const Event& event)
{
	// COLLISION ENTER
	if (event.id == make_sdbm_hash("collision_enter"))
	{

		if (m_Timer < CHECK_TIME)
			return;
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

				if (tile->GetFloor())
				{
					auto state = std::make_unique<LandedState>();
					m_pIngredientComponent->SetState(std::move(state));
				}

				if (tile->GetPlate())
				{
					auto state = std::make_unique<FallingOnPlate>();
					m_pIngredientComponent->SetState(std::move(state));
				}
			}
		}
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
	}
}

void dae::FallingState::OnExit(dae::GameObject&)
{
	if (m_Rigidbody)
	{
		m_Rigidbody->Stop();
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
