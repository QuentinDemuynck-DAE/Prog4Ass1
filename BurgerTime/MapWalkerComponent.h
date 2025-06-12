#pragma once

#include "Component.h"
#include <vector>
#include <memory>

#include "Globals.h"
#include "Observer.h"


namespace dae {

    class MapTileComponent;
    class GameObject;
    class CollisionComponent;
    class MapComponent;


    class MapWalkerComponent final : public Component
    {
    public:


        enum class ClimbDirection
        {
            UP,
            DOWN,
            BOTH,
            NONE
        };

        struct ActionQueryResult {
            bool    canPerform;      // true if the action is valid
            glm::vec3 snapPosition;    // where to snap the character
            ClimbDirection possibleDirections;
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
        ClimbDirection PossibleClimbDirections() const;

        bool IsOnFloor() const;

        const std::vector<MapTileComponent*>& GetConnectedTiles() const;

        void MoveTo(MapTileComponent& newTile);

        void Respawn();

        void AddTile(CollisionComponent* receiver);
        void RemoveTile(CollisionComponent* receiver);

        bool IsWayPossible(ClimbDirection climbingDirection, bool right) const; // Checks full path for a ladder
        bool IsNextAvailable(bool toRight) const; // checks one tile for a wall


    private:

        void KeepInBoundaries();
        void ResolveMapBounds(const glm::vec4& mapBounds);
        void ResolveWallCollision(const glm::vec4& wallBounds);
        glm::vec4 GetWalkerBounds() const;

        std::vector<MapTileComponent*> m_ConnectedTiles;
        CollisionComponent* m_OwnersCollisionComponent;
        glm::vec3 m_Spawnposition;
        MapComponent& m_Map;
    };

} 
