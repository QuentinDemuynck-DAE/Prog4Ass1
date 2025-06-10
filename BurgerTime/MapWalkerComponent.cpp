#include "MapWalkerComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "MapTileComponent.h"
#include "Subject.h"
#include "MapComponent.h"

namespace dae
{
	using ActionQueryResult = MapWalkerComponent::ActionQueryResult;

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



		m_OwnersCollisionComponent->AddCollisionLayerSelf(CollisionLayers::MAPWALKER);
		m_OwnersCollisionComponent->AddCollisionLayerToCollide(CollisionLayers::MAP);

		m_ConnectedTiles.clear();
	}

	dae::MapWalkerComponent::~MapWalkerComponent()
	{
	}


	void dae::MapWalkerComponent::Update(float)
	{
		KeepInBoundaries();
	}

	ActionQueryResult MapWalkerComponent::QueryClimbLadder() const
	{
		// Gather ladder exits
		std::vector<MapTileComponent*> exits;
		exits.reserve(m_ConnectedTiles.size());
		for (auto* tile : m_ConnectedTiles)
		{
			if (tile->GetLadderExit())
				exits.push_back(tile);
		}

		ActionQueryResult query{};
		if (exits.size() < 2)
			return query;

		MapTileComponent* leftExit = nullptr;
		MapTileComponent* rightExit = nullptr;

		// Look for a pair of adjacent tiles
		for (auto* tile : exits)
		{
			glm::ivec2 pos = tile->GetPositionInGrid();

			for (auto* otherTile : exits)
			{
				if (otherTile == tile) continue;

				glm::ivec2 otherPos = otherTile->GetPositionInGrid();
				if (otherPos.x == pos.x + 1 && otherPos.y == pos.y)
				{
					leftExit = tile;
					rightExit = otherTile;
					break;
				}
			}

			if (leftExit && rightExit)
				break;
		}

		if (!leftExit)
			return query;

		query.canPerform = true;

		// Snap to the X position of the left exit, but keep walker's Y and Z
		glm::vec3 leftWorld = leftExit->GetOwner()
			.GetTransform()
			->GetGlobalPosition();
		glm::vec3 walkerPos = GetOwner()
			.GetTransform()
			->GetLocalPosition();

		query.snapPosition = { leftWorld.x, walkerPos.y, walkerPos.z };
		return query;
	}


	ActionQueryResult MapWalkerComponent::QueryExitLadder() const
	{
		
		const auto walkerRect = m_OwnersCollisionComponent->GetRectangle();
		const float walkerTopY = walkerRect.y;
		const float walkerHeight = walkerRect.height;
		const float walkerBottomY = walkerTopY + walkerHeight;

		
		const float offset = walkerHeight * 0.25f;

		float bestExitY = 0.0f;
		float closestDelta = std::numeric_limits<float>::infinity();
		std::vector<MapTileComponent*> tileComponents;

		for (auto* tileComp : m_ConnectedTiles)
		{
			if (!tileComp->GetLadderExit())
				continue;

			auto* tileCollider = tileComp->GetOwner().GetComponent<CollisionComponent>();
			if (!tileCollider)
				continue;

	
			const auto tileRect = tileCollider->GetRectangle();
			const float exitBottomY = tileRect.y + tileRect.height;

			
			const float delta = std::abs(walkerBottomY - exitBottomY);
			if (delta <= offset)
			{
				tileComponents.push_back(tileComp);
				
				if (delta < closestDelta)
				{
					closestDelta = delta;
					bestExitY = exitBottomY;
				}
			}
		}

		ActionQueryResult query{};
		query.canPerform = (tileComponents.size() >= 2);
		if (!query.canPerform)
			return query;


		const glm::vec3 curr = GetOwner()
			.GetTransform()
			->GetGlobalPosition();

		query.snapPosition = {
			curr.x,
			bestExitY - (walkerHeight),
			curr.z
		};

		return query;
	}




	bool dae::MapWalkerComponent::IsOnFloor() const
	{
		if (m_ConnectedTiles.capacity() < m_ConnectedTiles.size())
			return false;

		for (auto& tile : m_ConnectedTiles)
		{
			if (tile->GetFloor())
			{
				return true;
			}
		}

		return false;
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

	void MapWalkerComponent::AddTile(CollisionComponent* receiver)
	{

		GameObject& otherGO = receiver->GetOwner();

		if (otherGO.HasComponent<MapTileComponent>())
		{
			auto tile = otherGO.GetComponent<MapTileComponent>();
			if (std::find(m_ConnectedTiles.begin(),
				m_ConnectedTiles.end(),
				tile)
				== m_ConnectedTiles.end())
			{
				m_ConnectedTiles.push_back(tile);
			}
		}
	}

	void MapWalkerComponent::RemoveTile(CollisionComponent* receiver)
	{

		GameObject& otherGO = receiver->GetOwner();

		if (otherGO.HasComponent<MapTileComponent>())
		{
			auto tile = otherGO.GetComponent<MapTileComponent>();
			auto it = std::find(m_ConnectedTiles.begin(),
				m_ConnectedTiles.end(),
				tile);
			if (it != m_ConnectedTiles.end())
			{
				m_ConnectedTiles.erase(it);
			}
		}
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
}


