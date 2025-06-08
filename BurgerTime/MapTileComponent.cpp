#include "MapTileComponent.h"

dae::MapTileComponent::MapTileComponent(GameObject& owner, const TileInfo& info, const glm::ivec2& gridPosition)
	:Component(owner),
	m_Position(gridPosition),
	m_Ladder(info.ladder),
	m_Floor(info.floor),
	m_LadderExit(info.ladderExit),
	m_Wall(info.wall)
{
}

void dae::MapTileComponent::Update(float dt)
{
	Component::Update(dt);
}

const glm::ivec2& dae::MapTileComponent::GetPositionInGrid() const
{
	return m_Position;
}
