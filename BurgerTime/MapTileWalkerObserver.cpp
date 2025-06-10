#include "MapTileWalkerObserver.h"

#include "CollisionComponent.h"
#include "GameObject.h"
#include "Globals.h"
#include "MapWalkerComponent.h"

dae::MapTileWalkerObserver::MapTileWalkerObserver(MapWalkerComponent* mapWalker)
	:m_MapWalkerComponent(mapWalker)
{
	
}

void dae::MapTileWalkerObserver::Notify(Event event, dae::GameObject* gameObject)
{
	if (!m_MapWalkerComponent)
		return;

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

			if (sender && receiver && sender == gameObject->GetComponent<CollisionComponent>())
				m_MapWalkerComponent->AddTile(receiver);
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

			if (sender && receiver && sender == gameObject->GetComponent<CollisionComponent>())
				m_MapWalkerComponent->RemoveTile(receiver);
		}
	}

}
