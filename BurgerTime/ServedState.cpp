#include "ServedState.h"

#include "GameManager.h"
#include "GameObject.h"
#include "Globals.h"
#include "Subject.h"

void dae::ServedState::OnEnter(dae::GameObject& gameObject)
{

	if (gameObject.HasComponent<IngredientComponent>())
	{
		m_pIngredientComponent = gameObject.GetComponent<IngredientComponent>();
	}


	Event event = Event(make_sdbm_hash("served"));

	gameObject.GetSubject()->Notify(event);
	GameManager::GetInstance().ServeIngredient();
}

void dae::ServedState::HandleInput(dae::GameObject&, const Event&)
{
}
