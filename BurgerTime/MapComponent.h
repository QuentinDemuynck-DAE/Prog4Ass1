// MapComponent.h
#pragma once
#include "Component.h"
#include "Box2D/Box2D.h"
#include "SVGParser.h"
#include <memory>
#include <vector>

namespace dae
{
	class MapTileComponent;
}

namespace dae
{
    class GameObject;




    class MapComponent final : public Component
    {
    public:

        struct surroundingTiles
        {
            MapTileComponent* above;
            MapTileComponent* below;
            MapTileComponent* left;
            MapTileComponent* right;
        };

        surroundingTiles GetSurroundingTiles(MapTileComponent* tile);

        MapComponent(GameObject& owner,
            b2World& world,
            const std::string& filename,
            const Vec2& offset);
        ~MapComponent() override = default;

        MapComponent(const MapComponent& other) = delete;
        MapComponent(MapComponent&& other) = delete;
        MapComponent& operator=(const MapComponent& other) = delete;
        MapComponent& operator=(MapComponent&& other) = delete;

        const std::vector<glm::vec2>& GetTopBunSpawnPositions() const;
        const std::vector<glm::vec2>& GetTomateSpawnPositions() const;
        const std::vector<glm::vec2>& GetSaladSpawnPositions() const;
        const std::vector<glm::vec2>& GetCheeseSpawnPositions() const;
        const std::vector<glm::vec2>& GetMeatSpawnPositions() const;
        const std::vector<glm::vec2>& GetBottomBunSpawnPositions() const;
        int GetTotalIngredients() const;
        const std::vector<glm::vec2>& GetPlayerSpawnPositions() const;

        const std::vector<glm::vec2>& GetSausageSpawnPositions() const;
        const std::vector<glm::vec2>& GetPickleSpawnPositions() const;
        const std::vector<glm::vec2>& GetEggSpawnPositions() const;



        const glm::vec4& Boundaries() const;


    private:
        // Yes I made a burger in my code :)
        std::vector<glm::vec2> m_TopBunSpawnPos;
        std::vector<glm::vec2> m_TomateSpawnPos;
        std::vector<glm::vec2> m_SaladSpawnPos;
        std::vector<glm::vec2> m_CheeseSpawnPos;
        std::vector<glm::vec2> m_MeatSpawnPos;
        std::vector<glm::vec2> m_BottomBunSpawnPos;
        int m_TotalIngredients;

        std::vector<glm::vec2> m_PlayerSpawnPos;
        std::vector<glm::vec2> m_SausageSpawnPos;
        std::vector<glm::vec2> m_PickleSpawnPos;
        std::vector<glm::vec2> m_EggSpawnPos;


        glm::vec4 m_Boundaries;

        b2World& m_World;
        Map      m_Map;

        // tiles
        std::vector<std::shared_ptr<GameObject>> m_TileObjects;
    };
}
