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


		auto below = m_Map.GetSurroundingTiles(leftExit).below;
		auto above = m_Map.GetSurroundingTiles(leftExit).above;
		bool up = false;
		bool down = false;

		if (below)
		{
			down = below->GetLadder();
		}

		if (above)
		{
			up = above->GetLadder();
		}

		query.possibleDirections = ClimbDirection::NONE;
		if (up && down)
		{
			query.possibleDirections = ClimbDirection::BOTH;
		}
		else if (up)
		{
			query.possibleDirections = ClimbDirection::UP;
		}
		else if (down)
		{
			query.possibleDirections = ClimbDirection::DOWN;

		}

		query.snapPosition = { leftWorld.x, walkerPos.y, walkerPos.z };
		return query;
	}


	ActionQueryResult MapWalkerComponent::QueryExitLadder() const
	{
		
		const auto walkerRect = m_OwnersCollisionComponent->GetRectangle();
		const float walkerTopY = walkerRect.y;
		const float walkerHeight = walkerRect.height;

		
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
			const float exitTopY = tileRect.y;

			
			const float delta = std::abs(walkerTopY - exitTopY);
			if (delta <= offset)
			{
				tileComponents.push_back(tileComp);
				
				if (delta < closestDelta)
				{
					closestDelta = delta;
					bestExitY = exitTopY;
				}
			}
		}

		ActionQueryResult query{};
		query.canPerform = (tileComponents.size() >= 2);
		if (!query.canPerform)
			return query;


		const glm::vec3 curr = GetOwner().GetTransform()->GetGlobalPosition();

		query.snapPosition = {
			curr.x,
			bestExitY,
			curr.z
		};

		return query;
	}


	bool MapWalkerComponent::IsWayPossible(ClimbDirection climbingDir, bool right) const
	{
		const int step = right ? +1 : -1;

		MapTileComponent* current = nullptr;

		for (auto tile : m_ConnectedTiles)
		{
			if (tile->GetLadderExit())
				current = tile;
		}

		for (int i = 0; i < 2 && current; ++i)
		{
			if (!current || current->GetWall())
				return false;

			auto neighbors = m_Map.GetSurroundingTiles(current);
			current = (step == +1 ? neighbors.right : neighbors.left);
		}

		if (!current || current->GetWall())
			return false;

		while (current)
		{
			auto neighbors = m_Map.GetSurroundingTiles(current);
			MapTileComponent* next = (step == +1 ? neighbors.right : neighbors.left);

			if (!next || next->GetWall())
				break;   // fell off the map

			if (next->GetLadderExit())
			{
				auto around = m_Map.GetSurroundingTiles(next);
				bool canUp = (around.above && around.above->GetLadder());
				bool canDown = (around.below && around.below->GetLadder());
				bool match = false;

				switch (climbingDir)
				{
				case ClimbDirection::UP:    match = canUp;             break;
				case ClimbDirection::DOWN:  match = canDown;           break;
				case ClimbDirection::BOTH:  match = (canUp && canDown); break;
				}

				if (match)
					return true;

				break;
			}

			current = next;
		}

		return false;
	}

	// Checks for one tile
	bool MapWalkerComponent::IsNextAvailable(bool toRight) const
	{
		// for each tile the walker is currently overlapping
		for (auto* tile : m_ConnectedTiles)
		{
			const auto neighbors = m_Map.GetSurroundingTiles(tile);
			MapTileComponent* next = toRight ? neighbors.right : neighbors.left;
			if (!next)
				continue;

			if (next->GetWall())
				return false;
		}
		return true;
	}


	MapWalkerComponent::ClimbDirection MapWalkerComponent::PossibleClimbDirections() const
	{
		std::vector<MapTileComponent*> exits;
		std::vector<MapTileComponent*> ladders;

		
		for (auto tile : m_ConnectedTiles)
		{
			if (tile->GetLadderExit())
				exits.push_back(tile);

			if (tile->GetLadder())
				ladders.push_back(tile);
		}

		auto highest = [](const std::vector<MapTileComponent*>& vec) -> MapTileComponent*
			{
				if (vec.empty()) return nullptr;
				return *std::max_element(
					vec.begin(), vec.end(),
					[](MapTileComponent* a, MapTileComponent* b) {
						return a->GetPositionInGrid().y < b->GetPositionInGrid().y;
					}
				);
			};

		auto lowest = [](const std::vector<MapTileComponent*>& vec) -> MapTileComponent*
			{
				if (vec.empty()) return nullptr;
				return *std::min_element(
					vec.begin(), vec.end(),
					[](MapTileComponent* a, MapTileComponent* b) {
						return a->GetPositionInGrid().y < b->GetPositionInGrid().y;
					}
				);
			};



		// get highest and lowest ladders and exits
		MapTileComponent* topLadder = highest(ladders);
		MapTileComponent* bottomLadder = lowest(ladders);
		MapTileComponent* topExit = highest(exits);
		MapTileComponent* bottomExit = lowest(exits);

		MapComponent::surroundingTiles topLadderSurrounding = m_Map.GetSurroundingTiles(topLadder);
		MapComponent::surroundingTiles bottomLadderSurrounding = m_Map.GetSurroundingTiles(bottomLadder);
		MapComponent::surroundingTiles topExitSurrounding = m_Map.GetSurroundingTiles(topExit);
		MapComponent::surroundingTiles bottomExitSurrounding = m_Map.GetSurroundingTiles(bottomExit);

		bool canClimbUp = false;
		bool canClimbDown = false;

		if (ladders.size() > 4)
		{
			return ClimbDirection::BOTH;
		}

		if (topExitSurrounding.above
			&& topExitSurrounding.above->GetLadder() || ladders.size() >= 4)
		{
			canClimbUp = true;
		}

		if (bottomExitSurrounding.below
			&& bottomExitSurrounding.below->GetLadder())
		{
			canClimbDown = true;
		}

		if (topLadderSurrounding.below
			&& topLadderSurrounding.below->GetLadderExit())
			canClimbUp = true;

		if (bottomLadderSurrounding.below
			&& bottomLadderSurrounding.below->GetLadderExit())
			canClimbDown = true;


		if (canClimbUp && canClimbDown)
			return ClimbDirection::BOTH;
		if (canClimbUp)
			return ClimbDirection::UP;
		if (canClimbDown)
			return ClimbDirection::DOWN;
		return ClimbDirection::NONE;
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


	void MapWalkerComponent::KeepInBoundaries()
	{
		// Enforce staying inside the overall map
		glm::vec4 mapBounds = m_Map.Boundaries();
		ResolveMapBounds(mapBounds);

		// Enforce staying outside any wall tiles
		for (auto tile : m_ConnectedTiles)
		{
			if (!tile->GetWall())
				continue;

			glm::vec4 boundary
			{
				tile->GetOwner().GetTransform()->GetGlobalPosition().x,
				tile->GetOwner().GetTransform()->GetGlobalPosition().y,
				tile->GetOwner().GetTransform()->GetGlobalPosition().x + tile->GetOwner().GetComponent<CollisionComponent>()->GetRectangle().width,
				tile->GetOwner().GetTransform()->GetGlobalPosition().y + tile->GetOwner().GetComponent<CollisionComponent>()->GetRectangle().height,
			};

			ResolveWallCollision(boundary);
		}
	}


	glm::vec4 MapWalkerComponent::GetWalkerBounds() const
	{
		// Build the walker’s world-space AABB
		const Rectangle localRect = m_OwnersCollisionComponent->GetRectangle();


		return {
			GetOwner().GetTransform()->GetGlobalPosition().x,
			GetOwner().GetTransform()->GetGlobalPosition().y,
			GetOwner().GetTransform()->GetGlobalPosition().x + localRect.width,
			GetOwner().GetTransform()->GetGlobalPosition().y + localRect.height
		};
	}

	void MapWalkerComponent::ResolveMapBounds(const glm::vec4& mapBounds)
	{
		
		glm::vec4 walkerBounds = GetWalkerBounds();
		// Calculate how far the walker is outside each side of the map
		float overlapX = 0.0f;
		if (walkerBounds.x < mapBounds.x)
			overlapX = mapBounds.x - walkerBounds.x;
		else if (walkerBounds.z > mapBounds.z)
			overlapX = mapBounds.z - walkerBounds.z;

		float overlapY = 0.0f;
		if (walkerBounds.y < mapBounds.y)
			overlapY = mapBounds.y - walkerBounds.y;
		else if (walkerBounds.w > mapBounds.w)
			overlapY = mapBounds.w - walkerBounds.w;

		// Push the walker back inside the map
		if (overlapX != 0.0f || overlapY != 0.0f)
		{
			glm::vec3 correctedPosition = GetOwner().GetTransform()->GetGlobalPosition() + glm::vec3(overlapX, overlapY, 0.0f);
			GetOwner().GetTransform()->SetLocalPosition(correctedPosition);

			Event event = Event(make_sdbm_hash("out_of_bounds"));
			GetOwner().GetSubject()->Notify(event);
		}
	}

	void MapWalkerComponent::ResolveWallCollision(const glm::vec4& wallBounds)
	{
		// walkerBounds: x = left, y = top z = right, w = bottom
		glm::vec4 walker = GetWalkerBounds();

		const float VERTICAL_THRESHOLD{ 7 };

		float topPen = wallBounds.y - walker.w;
		float botPen = walker.y - wallBounds.w;

		if (!bool(bool(-topPen > VERTICAL_THRESHOLD && -topPen < walker.w - walker.y) || bool(-botPen > VERTICAL_THRESHOLD && -botPen < walker.w - walker.y)))
			return;

		float leftPen = walker.z - wallBounds.x;
		float rightPen = wallBounds.z - walker.x;

		leftPen = (leftPen > 0.0f) ? (leftPen) : FLT_MAX;
		rightPen = (rightPen > 0.0f) ? (rightPen) : FLT_MAX;

		glm::vec3 correctedPos{};

		if (rightPen > walker.w - walker.y && leftPen > walker.w - walker.y)
			return;

		if (leftPen < rightPen)
		{
			correctedPos.x = GetOwner().GetTransform()->GetGlobalPosition().x - leftPen;
		}
		else
		{
			correctedPos.x = GetOwner().GetTransform()->GetGlobalPosition().x + rightPen;

		}

		Event event = Event(make_sdbm_hash("out_of_bounds"));
		GetOwner().GetSubject()->Notify(event);

		GetOwner().GetTransform()->SetLocalPosition(
		{
			correctedPos.x,
			GetOwner().GetTransform()->GetGlobalPosition().y,
			GetOwner().GetTransform()->GetGlobalPosition().z
		}
		);

	}

}


