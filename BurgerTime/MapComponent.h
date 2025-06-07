// MapComponent.h
#pragma once
#include "Component.h"
#include "Box2D/Box2D.h"
#include "SVGParser.h"
#include <memory>
#include <vector>

namespace dae
{
    class GameObject;

    class MapComponent final : public Component
    {
    public:
        MapComponent(GameObject& owner,
            b2World& world,
            const std::string& filename,
            const Vec2& offset);
        ~MapComponent() override = default;

        MapComponent(const MapComponent& other) = delete;
        MapComponent(MapComponent&& other) = delete;
        MapComponent& operator=(const MapComponent& other) = delete;
        MapComponent& operator=(MapComponent&& other) = delete;

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


        b2World& m_World;
        Map      m_Map;

        // tiles
        std::vector<std::shared_ptr<GameObject>> m_TileObjects;
    };
}
