#include "EnemyObserver.h"

#include "EnemyComponent.h"
#include "GameObject.h"

EnemyObserver::EnemyObserver()
{
}

void EnemyObserver::Notify(Event event, dae::GameObject* gameObject)
{
	gameObject->GetComponent<EnemyComponent>()->HandleInput(event);
}
