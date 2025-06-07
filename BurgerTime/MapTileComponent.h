#pragma once

#include "Component.h"
#include <glm.hpp>
#include "SVGParser.h"

namespace dae {

    class GameObject;
    class MapTileComponent final : public Component
    {
    public:
        MapTileComponent(GameObject& owner,
            const TileInfo& info,
            const glm::ivec2& position);

        ~MapTileComponent() override = default;


        MapTileComponent(const MapTileComponent&) = delete;
        MapTileComponent& operator=(const MapTileComponent&) = delete;
        MapTileComponent(MapTileComponent&& other) = delete;
        MapTileComponent& operator=(MapTileComponent&& other) = delete;

        void Update(float dt) override;

        const glm::ivec2& GetPosition() const;

        bool GetLadder() const { return m_Ladder; }
        bool GetFloor() const { return m_Floor; }
        bool GetLadderExit() const { return m_LadderExit; }
        bool GetWall() const { return m_Wall; }

    private:
        glm::ivec2  m_Position; // as in row and column
        bool m_Ladder;
        bool m_Floor;
        bool m_LadderExit;
        bool m_Wall;
    };

}
