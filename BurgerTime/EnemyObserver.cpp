#include "EnemyObserver.h"

#include "EnemyComponent.h"
#include "GameObject.h"
#include "Globals.h"
EnemyObserver::EnemyObserver()
{
}

void EnemyObserver::Notify(Event event, dae::GameObject* gameObject)
{
	gameObject->GetComponent<EnemyComponent>()->HandleInput(event);

	if (event.id == make_sdbm_hash("collision_enter"))
	{
		std::cout << "Boem boem collided" << std::endl;
	}
}
