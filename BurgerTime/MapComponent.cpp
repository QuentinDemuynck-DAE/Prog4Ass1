#include "MapComponent.h"
#include "CollisionComponent.h"
#include "MapTileComponent.h"
#include "GameObject.h"

dae::MapComponent::surroundingTiles dae::MapComponent::GetSurroundingTiles(MapTileComponent* tile)
{
	if (!tile)
		return surroundingTiles{};

	glm::ivec2 position = tile->GetPositionInGrid();

	surroundingTiles surroundingTiles;

	// Lambda to find neighbors
	auto findNeighbor = [&](glm::ivec2 offset) -> MapTileComponent*
		{
			glm::ivec2 target = position + offset;
			auto it = std::find_if(
				m_TileObjects.begin(),
				m_TileObjects.end(),
				[&](const std::shared_ptr<GameObject>& other) {
					auto tileComp = other->GetComponent<MapTileComponent>();
					return tileComp && tileComp->GetPositionInGrid() == target;
				}
			);
			return (it != m_TileObjects.end()) ? (*it)->GetComponent<MapTileComponent>() : nullptr;
		};

	// Calling the lambda on all the surrounding tiles
	surroundingTiles.above = findNeighbor(glm::ivec2{0,-1});
	surroundingTiles.below = findNeighbor(glm::ivec2{ 0,1 });
	surroundingTiles.left = findNeighbor(glm::ivec2{ -1,0 });
	surroundingTiles.right = findNeighbor(glm::ivec2{ 1,0 });

	return surroundingTiles;

}

dae::MapComponent::MapComponent(GameObject& owner, b2World& world, const std::string& filename,
                                const Vec2& offset)
	:Component(owner),
	m_World(world)
{
	m_Map = loadMapFromTiledJSON(filename, offset);
	// So I don't have to write it out all the time
	const int cols = m_Map.columns;
	const int rows = m_Map.rows;
	const float tileW = m_Map.tileSize.x;
	const float tileH = m_Map.tileSize.y;
	const Vec2  origin = Vec2(offset.x, offset.y);


	// store boundaries
	const auto transform = owner.GetTransform();
	glm::vec2    scale2D = transform->GetGlobalScale();
	glm::vec3    ownerPos3 = transform->GetGlobalPosition();
	glm::vec2    ownerPos2 = glm::vec2{ ownerPos3.x, ownerPos3.y };

	glm::vec2 scaledMargin = scale2D * m_Map.margin;

	glm::vec2 worldMin = origin + ownerPos2;
	glm::vec2 tileSizeWS = glm::vec2{ tileW, tileH } *scale2D;
	glm::vec2 worldMax = worldMin + glm::vec2{ cols * tileSizeWS.x,
												  rows * tileSizeWS.y };

	m_Boundaries = glm::vec4{ worldMin.x,
							  worldMin.y,
							  worldMax.x,
							  worldMax.y -2 }; // Little offset on the top so you don't glitch in the ceiling whoops


	m_TileObjects.reserve(cols * rows);

	// double for loop over the grid
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			
			int index = y * cols + x;
			const TileInfo& info = m_Map.tiles[index]; // get the correct tile
			if (!info.floor && !info.wall && !info.ladder && !info.ladderExit && !info.bottomBun && !info.topBun && !info.plate && !info.meat && !info.cheese && !info.salad && !info.tomato && !info.playerSpawn) // basically useless tile
				continue;
			
			glm::vec2 scale = owner.GetTransform()->GetGlobalScale();
			glm::vec3 ownerPos = owner.GetTransform()->GetGlobalPosition();
			glm::vec2 scaledTileSize = glm::vec2{ tileW, tileH } *glm::vec2{ scale.x, scale.y };
			glm::vec3 scaledPosition = glm::vec3{ origin, 0 } + glm::vec3{ x * scaledTileSize.x, y * scaledTileSize.y, 0.0f };
			scaledPosition.x += scaledMargin.x;
			scaledPosition.y += scaledMargin.y;



			// yet another burger this time in reverse
			if (info.bottomBun)
			{
				m_BottomBunSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}
			else if (info.meat)
			{
				m_MeatSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}
			else if (info.cheese)
			{
				m_CheeseSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}
			else if (info.salad)
			{
				m_SaladSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}
			else if (info.tomato)
			{
				m_TomateSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}
			else if (info.topBun)
			{
				m_TopBunSpawnPos.push_back(scaledPosition + ownerPos);
				m_TotalIngredients++;
			}

			if (info.playerSpawn)
			{
				m_PlayerSpawnPos.push_back(scaledPosition + ownerPos);
			}
			if (info.sausage)
			{
				m_SausageSpawnPos.push_back(scaledPosition + ownerPos);
			}
			if (info.pickle)
			{
				m_PickleSpawnPos.push_back(scaledPosition + ownerPos);
			}
			if (info.egg)
			{
				m_EggSpawnPos.push_back(scaledPosition + ownerPos);
			}

			// Add a tile component with hitbox
			if (!info.floor && !info.wall && !info.ladder && !info.ladderExit && !info.plate)
				continue;

			glm::vec2 halfSize{ tileW / 2, tileH / 2 };
			halfSize *= glm::vec2{ owner.GetTransform()->GetGlobalScale().x , owner.GetTransform()->GetGlobalScale().y };

			auto gameObject = std::make_shared<dae::GameObject>(scaledPosition);
			gameObject->AddComponent<CollisionComponent>(world, dae::CollisionLayers::MAP, dae::CollisionLayers::MAPWALKER | CollisionLayers::INGREDIENT, halfSize, glm::vec2{0,0}, true, false);
			gameObject->AddComponent<MapTileComponent>(info,glm::ivec2{x,y});
			gameObject->SetParent(&owner);

			m_TileObjects.push_back(gameObject);
		}
	}
}


const std::vector<glm::vec2>& dae::MapComponent::GetTopBunSpawnPositions() const {
	return m_TopBunSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetTomateSpawnPositions() const {
	return m_TomateSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetSaladSpawnPositions() const {
	return m_SaladSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetCheeseSpawnPositions() const {
	return m_CheeseSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetMeatSpawnPositions() const {
	return m_MeatSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetBottomBunSpawnPositions() const {
	return m_BottomBunSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetPlayerSpawnPositions() const {
	return m_PlayerSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetSausageSpawnPositions() const
{
	return m_SausageSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetPickleSpawnPositions() const
{
	return m_PickleSpawnPos;
}

const std::vector<glm::vec2>& dae::MapComponent::GetEggSpawnPositions() const
{
	return m_EggSpawnPos;

}

const glm::vec4& dae::MapComponent::Boundaries() const
{
	return m_Boundaries;
}

int dae::MapComponent::GetTotalIngredients() const {
	return m_TotalIngredients;
}
