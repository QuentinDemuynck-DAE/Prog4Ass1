#pragma once

#include "Component.h"
#include <vector>
#include <memory>

namespace dae {

    class MapTileComponent;
    class GameObject;

    class MapWalkerComponent final : public Component
    {
    public:
        MapWalkerComponent(GameObject& owner,
            MapTileComponent& startTile);

        ~MapWalkerComponent() override = default;

        MapWalkerComponent(const MapWalkerComponent&) = delete;
        MapWalkerComponent& operator=(const MapWalkerComponent&) = delete;
        MapWalkerComponent(MapWalkerComponent&& other) = delete;
        MapWalkerComponent& operator=(MapWalkerComponent&& other) = delete;


        void Update(float dt) override;
        void Render(glm::vec3 position, glm::vec2 scale) override;


        const std::vector<MapTileComponent*>& GetConnectedTiles() const;
        bool MoveTo(MapTileComponent& newTile);
        void RefreshConnections();

    private:
        std::vector<MapTileComponent*> m_ConnectedTiles;
    };

} 
