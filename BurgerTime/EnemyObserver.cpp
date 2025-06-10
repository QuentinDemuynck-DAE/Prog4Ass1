#include "EnemyObserver.h"

#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"

namespace dae
{
	EnemyObserver::EnemyObserver()
	{
	}

	void EnemyObserver::Notify(Event event, dae::GameObject* gameObject)
	{
		gameObject->GetComponent<EnemyComponent>()->HandleInput(event);

	}
}

