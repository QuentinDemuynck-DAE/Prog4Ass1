#pragma once

#include "Component.h"
#include <vector>
#include <memory>

#include "Observer.h"


namespace dae {

    class MapTileComponent;
    class GameObject;
    class CollisionComponent;
    class MapComponent;


    class MapWalkerComponent final : public Component, public Observer
    {
    public:
        MapWalkerComponent(GameObject& owner, glm::vec3 spawnPosition, MapComponent& mapComponent);

        ~MapWalkerComponent() override = default;

        MapWalkerComponent(const MapWalkerComponent&) = delete;
        MapWalkerComponent& operator=(const MapWalkerComponent&) = delete;
        MapWalkerComponent(MapWalkerComponent&& other) = delete;
        MapWalkerComponent& operator=(MapWalkerComponent&& other) = delete;


        void Notify(Event event, dae::GameObject* gameObject) override;
        void Update(float deltaTime) override;

        bool CanTakeLadder() const;
        bool CanExitLadder() const;

        const std::vector<MapTileComponent*>& GetConnectedTiles() const;

        void MoveTo(MapTileComponent& newTile);

        void Respawn();

    private:

        void KeepInBoundaries();

        std::vector<MapTileComponent*> m_ConnectedTiles;
        CollisionComponent* m_OwnersCollisionComponent;
        glm::vec3 m_Spawnposition;
        MapComponent& m_Map;
    };

} 
