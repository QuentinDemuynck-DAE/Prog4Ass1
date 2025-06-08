#include "MapWalkerComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "MapTileComponent.h"
#include "Subject.h"
#include "MapComponent.h"

dae::MapWalkerComponent::MapWalkerComponent(GameObject& owner, glm::vec3 spawnPosition, MapComponent& map)
	:Component(owner),
	m_Spawnposition(spawnPosition),
	m_Map(map)
{
	if (owner.HasComponent<CollisionComponent>())
	{
		m_OwnersCollisionComponent = owner.GetComponent<CollisionComponent>();
	}


	if (!m_OwnersCollisionComponent)
	{
		std::cout << "Owner of MapWalkerComponent doesn't have a collision component" << std::endl;
		return;
	}

	auto self = std::shared_ptr<Observer>(this,
		[](Observer*) {});

	owner.GetSubject()->AddObserver(self);

	m_OwnersCollisionComponent->AddCollisionLayerSelf(CollisionLayers::MAPWALKER);
	m_OwnersCollisionComponent->AddCollisionLayerToCollide(CollisionLayers::MAP);



}

void dae::MapWalkerComponent::Notify(Event event, dae::GameObject*)
{
	// ADD
	if (event.id == make_sdbm_hash("collision_enter"))
	{
		if (event.numberArgs >= 2 && std::holds_alternative<void*>(event.args[1]))
		{
			void* raw = std::get<void*>(event.args[1]);
			auto otherCollider = static_cast<dae::CollisionComponent*>(raw);
			GameObject& recoveredGO = otherCollider->GetOwner();

			if (recoveredGO.HasComponent<MapTileComponent>())
			{
				auto tileComp = recoveredGO.GetComponent<MapTileComponent>();
				if (std::find(m_ConnectedTiles.begin(), m_ConnectedTiles.end(), tileComp)
					== m_ConnectedTiles.end())
				{
					m_ConnectedTiles.push_back(tileComp);
				}
			}
		}
	}

	// REMOVE
	if (event.id == make_sdbm_hash("collision_exit"))
	{
		if (event.numberArgs >= 2 && std::holds_alternative<void*>(event.args[1]))
		{
			void* raw = std::get<void*>(event.args[1]);
			auto otherCollider = static_cast<dae::CollisionComponent*>(raw);
			GameObject& recoveredGO = otherCollider->GetOwner();

			if (recoveredGO.HasComponent<MapTileComponent>())
			{
				auto tileComp = recoveredGO.GetComponent<MapTileComponent>();
				auto it = std::find(m_ConnectedTiles.begin(),
					m_ConnectedTiles.end(),
					tileComp);
				if (it != m_ConnectedTiles.end())
					m_ConnectedTiles.erase(it);
			}
		}
	}


}

void dae::MapWalkerComponent::Update(float)
{
	KeepInBoundaries();
}

bool dae::MapWalkerComponent::CanTakeLadder() const
{
	int amountOfExits{ 0 };

	for (auto tile : m_ConnectedTiles)
	{
		if (tile->GetLadderExit())
		{
			amountOfExits++;
		}
	}

	// You can only start climbing once you hit both so youre atleast kinda at the center
	return amountOfExits >= 2;
}

bool dae::MapWalkerComponent::CanExitLadder() const
{
	int amountOfExits{ 0 };
	int amountOfFloor{ 0 };
	for (auto tile : m_ConnectedTiles)
	{
		if (tile->GetLadderExit())
		{
			amountOfExits++;
		}
		if (tile->GetFloor())
		{
			amountOfFloor++;
		}
	}

	// You have to be on an exit + not on the floor (to avoid getting of from underneath , as the floor tiles are underneath the actual floor image this probably wont bother)
	return (amountOfExits >= 2 && amountOfFloor == 0);
}

const std::vector<dae::MapTileComponent*>& dae::MapWalkerComponent::GetConnectedTiles() const
{
	return m_ConnectedTiles;
}

void dae::MapWalkerComponent::MoveTo(MapTileComponent& newTile)
{
	GetOwner().GetTransform()->SetLocalPosition(newTile.GetOwner().GetTransform()->GetGlobalPosition());
}

void dae::MapWalkerComponent::Respawn()
{
	m_ConnectedTiles.clear();
	GetOwner().GetTransform()->SetLocalPosition(m_Spawnposition);
}

void dae::MapWalkerComponent::KeepInBoundaries()
{
	
	glm::vec4 mapBoundaries = m_Map.Boundaries();
	dae::Rectangle walkerRectangle = m_OwnersCollisionComponent->GetRectangle();

	auto* walkerTransform = GetOwner().GetTransform();
	glm::vec3 walkerPosition = walkerTransform->GetLocalPosition();

	//basically how much hes over the map
	float penetrationX = 0.0f;
	float penetrationY = 0.0f;

	//Left
	if (walkerRectangle.x < mapBoundaries.x)
	{
		penetrationX = mapBoundaries.x - walkerRectangle.x;
	}
	// Right
	else if (walkerRectangle.x + walkerRectangle.width > mapBoundaries.z)
	{
		penetrationX = mapBoundaries.z - (walkerRectangle.x + walkerRectangle.width);
	}

	//Bottom
	if (walkerRectangle.y < mapBoundaries.y)
	{
		penetrationY = mapBoundaries.y - walkerRectangle.y;
	}
	//Top
	else if (walkerRectangle.y + walkerRectangle.height > mapBoundaries.w)
	{
		penetrationY = mapBoundaries.w - (walkerRectangle.y + walkerRectangle.height);
	}

	// If there is any overlap, push the walker back inside the map
	if (penetrationX != 0.0f || penetrationY != 0.0f)
	{
		walkerPosition.x += penetrationX;
		walkerPosition.y += penetrationY;
		walkerTransform->SetLocalPosition(walkerPosition);
	}
}

