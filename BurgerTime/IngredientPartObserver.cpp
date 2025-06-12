#include "IngredientPartObserver.h"
#include "GameObject.h"
#include "Globals.h"
#include "IngredientPartComponent.h"
#include "CollisionComponent.h"
#include "Subject.h"

namespace dae
{
	class CollisionComponent;
}

dae::IngredientPartObserver::IngredientPartObserver(GameObject* pIngredientObject)
	:m_pIngredientObject(pIngredientObject)
{
}

void dae::IngredientPartObserver::Notify(Event event, dae::GameObject* gameObject)
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

			if (!(sender && receiver && sender == gameObject->GetComponent<CollisionComponent>()))
				return;


		}
	}

	// COLLISION EXIT
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

			if (!(sender && receiver && sender == gameObject->GetComponent<CollisionComponent>()))
				return;


			auto ingredientPartComponent = gameObject->GetComponent<IngredientPartComponent>();

			if (!ingredientPartComponent->Activated())
			{
				ingredientPartComponent->Activate();
				Event eventToActivate = Event(make_sdbm_hash("part_activated"));
				m_pIngredientObject->GetSubject()->Notify(eventToActivate);

			}

		}
	}
}
