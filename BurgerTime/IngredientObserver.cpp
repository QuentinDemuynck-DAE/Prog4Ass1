#include "IngredientObserver.h"

#include "GameObject.h"
#include "IngredientComponent.h"

void dae::IngredientObserver::Notify(Event event, dae::GameObject* gameObject)
{
	if (gameObject->HasComponent<IngredientComponent>())
	{
		gameObject->GetComponent<IngredientComponent>()->HandleInput(event);
	}
}
