#pragma once
#include "Collision.h"
#include "Component.h"



class dae::GameObject;

class CollisionComponent : public Component
{
public:
    CollisionComponent(dae::GameObject& owner,
        b2World& world,
        const glm::vec2& halfSize,
        const glm::vec2& offset = glm::vec2{ 0.0f, 0.0f },
        bool dynamic = false,
        bool isSensor = true
    );

    ~CollisionComponent() override;

    void Update(float /*dt*/) override;

    /// Called by your ContactListener when another body enters this sensor
    void OnTriggerEnter(CollisionComponent* other);

    /// …and when it leaves…
    void OnTriggerExit(CollisionComponent* other);

    /// Expose the underlying body/fixture if you need it
    b2Body* GetBody()    const { return m_pBody; }
    b2Fixture* GetFixture() const { return m_pFixture; }

    void Render(glm::vec3 position) override;

private:
    b2World& m_World;
    b2Body* m_pBody = nullptr;
    b2Fixture* m_pFixture = nullptr;
    glm::vec2   m_HalfSize;
    glm::vec2   m_Offset;
#if defined(_DEBUG) || !defined(NDEBUG)
    bool m_DrawDebug = true;
#else
    bool m_DrawDebug = false;
#endif
};


