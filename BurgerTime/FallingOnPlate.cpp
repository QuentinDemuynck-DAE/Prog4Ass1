#include "FallingOnPlate.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "IngredientComponent.h"
#include "MapTileComponent.h"
#include "RigidbodyComponent.h"
#include "ServedState.h"

void dae::FallingOnPlate::HandleInput(GameObject& gameObject, const Event& event)
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

				if (tile->GetFloor())
				{
					auto state = std::make_unique<ServedState>();
					m_pIngredientComponent->SetState(std::move(state));
				}
			}
			if (gob.HasComponent<IngredientComponent>() && m_pIngredientComponent)
			{

				auto state = std::make_unique<ServedState>();
				m_pIngredientComponent->SetState(std::move(state));
			}
		}
	}
}

void dae::FallingOnPlate::OnEnter(GameObject& gameObject)
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

void dae::FallingOnPlate::Update(GameObject&, float)
{
	if (m_Rigidbody)
	{
		m_Rigidbody->AddVelocity(m_Speed);
	}
}

void dae::FallingOnPlate::OnExit(GameObject&)
{
	if (m_Rigidbody)
	{
		m_Rigidbody->Stop();
	}
}
