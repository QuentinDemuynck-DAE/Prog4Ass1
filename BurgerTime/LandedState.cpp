#include "LandedState.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "FallingState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include "IngredientComponent.h"
#include "Subject.h"

void dae::LandedState::OnEnter(dae::GameObject& gameObject)
{
	if (gameObject.HasComponent<IngredientComponent>())
	{
		m_pIngredientComponent = gameObject.GetComponent<IngredientComponent>();
	}

	if (m_pIngredientComponent)
	{
		m_pIngredientComponent->SetActivatableAllPArts(true);
		m_pIngredientComponent->ClearEnemiesStanding();
	}

	GameManager::GetInstance().AddScore(50);

}

void dae::LandedState::OnExit(dae::GameObject& gameObject)
{
	if (m_pIngredientComponent)
	{
		m_pIngredientComponent->SetActivatableAllPArts(false);
		m_pIngredientComponent->DeactivateAllChilds();

		glm::vec3 newPosition = m_pIngredientComponent->GetOwner().GetTransform()->GetGlobalPosition();
		newPosition.y += m_pIngredientComponent->GetActivationFalldown();
		m_pIngredientComponent->GetOwner().GetTransform()->SetLocalPosition(newPosition);

		Event event = Event(make_sdbm_hash("ingredient_started_falling"));
		event.AddArg(&gameObject);

		for (auto& enemy : m_pIngredientComponent->GetEnemiesStanding())
		{
			enemy->GetSubject()->Notify(event);
		}
	}

	

}

void dae::LandedState::HandleInput(dae::GameObject& gameObject, const Event& event)
{
	if (event.id == make_sdbm_hash("part_activated"))
	{
		++numberOfActivations;
	}

	if (!m_pIngredientComponent)
		return;

	if (numberOfActivations >= m_pIngredientComponent->GetNumChilds())
	{
		auto state = std::make_unique<FallingState>();
		m_pIngredientComponent->SetState(std::move(state));
	}

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

			if (receiver->GetOwner().HasComponent<EnemyComponent>()) //Add enemies 
			{
				m_pIngredientComponent->AddEnemyStanding(&receiver->GetOwner());
			}

			if (receiver->GetOwner().HasComponent<IngredientComponent>()) //Collided with a falling ingredient so also fall down
			{
				auto state = std::make_unique<FallingState>();
				m_pIngredientComponent->SetState(std::move(state));
			}

		}
	}


	// COLLISION exit
	if (event.id == make_sdbm_hash("collision_exit"))
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

			if (receiver->GetOwner().HasComponent<EnemyComponent>()) //Add enemies 
			{
				m_pIngredientComponent->RemoveEnemyStanding(&receiver->GetOwner());
			}
		}
	}

}
