#pragma once
#include <vector>

#include "Collision.h"
#include "Component.h"
#include "Globals.h"

namespace dae
{
    class GameObject;

    class CollisionComponent : public Component
    {
    public:
        CollisionComponent(dae::GameObject& owner,
            b2World& world,
            dae::CollisionLayers collisionLayerSelf,
            dae::CollisionLayers collisionLayerToCollide,
            const glm::vec2& halfSize,
            const glm::vec2& offset = glm::vec2{ 0.0f, 0.0f },
            bool dynamic = false,
            bool isSensor = true
        );

        ~CollisionComponent() override;

        void Update(float /*dt*/) override;

        void OnTriggerEnter(CollisionComponent* other);

        void OnTriggerExit(CollisionComponent* other);

        b2Body* GetBody()    const { return m_pBody; }
        b2Fixture* GetFixture() const { return m_pFixture; }

        void Render(glm::vec3 position, glm::vec2 scale) override;

        void AddCollisionLayerSelf(dae::CollisionLayers);
        void AddCollisionLayerToCollide(dae::CollisionLayers);

        void RemoveCollisionLayerToSelf(dae::CollisionLayers);
        void RemoveCollisionLayerToCollide(dae::CollisionLayers);

        Rectangle GetRectangle() const;

        void Activate();
        void Deactivate();

    private:

        void UpdateCollisionLayers();
        b2World& m_World;
        b2Body* m_pBody = nullptr;
        b2Fixture* m_pFixture = nullptr;
        glm::vec2   m_HalfSize;
        glm::vec2   m_Offset;
        dae::CollisionLayers m_SelfLayers = dae::CollisionLayers::NONE;
        dae::CollisionLayers m_CollidesWithLayers = dae::CollisionLayers::NONE;

#if defined(_DEBUG) || !defined(NDEBUG)
        bool m_DrawDebug = true;
#else
        bool m_DrawDebug = false;
#endif
    };
}




