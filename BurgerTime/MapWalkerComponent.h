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


    class MapWalkerComponent final : public Component
    {
    public:

        struct ActionQueryResult {
            bool    canPerform;      // true if the action is valid
            glm::vec3 snapPosition;    // where to snap the character
        };

        MapWalkerComponent(GameObject& owner, glm::vec3 spawnPosition, MapComponent& mapComponent);

        ~MapWalkerComponent() override;

        MapWalkerComponent(const MapWalkerComponent&) = delete;
        MapWalkerComponent& operator=(const MapWalkerComponent&) = delete;
        MapWalkerComponent(MapWalkerComponent&& other) = delete;

        MapWalkerComponent& operator=(MapWalkerComponent&& other) = delete;
        void Update(float deltaTime) override;

        ActionQueryResult QueryClimbLadder() const;
        ActionQueryResult QueryExitLadder() const;
        bool IsOnFloor() const;

        const std::vector<MapTileComponent*>& GetConnectedTiles() const;

        void MoveTo(MapTileComponent& newTile);

        void Respawn();

        void AddTile(CollisionComponent* receiver);
        void RemoveTile(CollisionComponent* receiver);


    private:
        void KeepInBoundaries();
        std::vector<MapTileComponent*> m_ConnectedTiles;
        CollisionComponent* m_OwnersCollisionComponent;
        glm::vec3 m_Spawnposition;
        MapComponent& m_Map;
    };

} 
