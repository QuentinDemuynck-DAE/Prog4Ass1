#include "CollisionComponent.h"

#include "Events.h"
#include "GameObject.h"
#include "Globals.h"
#include "Renderer.h"
#include "Subject.h"

CollisionComponent::CollisionComponent(dae::GameObject& owner,
    b2World& world,
    const glm::vec2& halfSize,
    const glm::vec2& offset,
    bool dynamic,
    bool isSensor)
    : Component(owner)
    , m_World(world)
{
    glm::vec2 globalScale{
        owner.GetTransform()->GetGlobalScale().x,
        owner.GetTransform()->GetGlobalScale().y
    };

    m_HalfSize = halfSize * globalScale;

    glm::vec2 scaledOffset = offset * globalScale;

    b2BodyDef bd;
    bd.type = dynamic ? b2_dynamicBody : b2_staticBody;

    glm::vec3 pos3 = owner.GetTransform()->GetGlobalPosition();
    bd.position.Set(pos3.x, pos3.y);

    glm::vec3 eulerDeg = owner.GetTransform()->GetGlobalRotation();
    float angleRad = glm::radians(eulerDeg.z);
    bd.angle = angleRad;

    bd.userData = this;
    m_pBody = m_World.CreateBody(&bd);


    b2PolygonShape shape;
    shape.SetAsBox(
        m_HalfSize.x,
        m_HalfSize.y,
         b2Vec2{ scaledOffset.x, scaledOffset.y },
         0.0f
    );

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.isSensor = isSensor;
    m_pFixture = m_pBody->CreateFixture(&fd);
}


CollisionComponent::~CollisionComponent()
{
    if (m_pBody)
    {
        m_World.DestroyBody(m_pBody);
    }
}

void CollisionComponent::Update(float)
{
    // Snap the Box2D body to match the GameObject transform each frame
    glm::vec3 pos = GetOwner().GetTransform()->GetGlobalPosition();
    glm::vec3 eulerDeg = GetOwner().GetTransform()->GetGlobalRotation();
    float   angleRad = glm::radians(eulerDeg.z);

    m_pBody->SetTransform(b2Vec2{ pos.x + m_Offset.x, pos.y + m_Offset.y }, angleRad);
}


void CollisionComponent::OnTriggerEnter(CollisionComponent* other)
{
    std::cout << "trigger enter" << std::endl;

    Event e{ make_sdbm_hash("collision_enter") };
    e.AddArg(this);
    e.AddArg(other);
    GetOwner().GetSubject()->Notify(e);
}

void CollisionComponent::OnTriggerExit(CollisionComponent* other)
{
    std::cout << "trigger exit" << std::endl;

    Event e{ make_sdbm_hash("collision_exit") };
    e.AddArg(this);
    e.AddArg(other);
    GetOwner().GetSubject()->Notify(e);
}

void CollisionComponent::Render(glm::vec3 position, glm::vec2)
{
    Component::Render(position, glm::vec2{1,1}); // m_HalfSize gets scaled

    if (!m_DrawDebug || !m_pFixture)
        return;

    auto* boxShape = static_cast<b2PolygonShape*>(m_pFixture->GetShape());
    int32 count = boxShape->m_count;
    auto* verts = boxShape->m_vertices;

    glm::vec2 firstPt{};
    glm::vec2 prevPt{};

    for (int32 i = 0; i < count; ++i)
    {
        b2Vec2 vWorld = m_pBody->GetWorldPoint(verts[i]);
        glm::vec2 currPt{ vWorld.x, vWorld.y };

        if (i == 0)
        {
            firstPt = currPt;
        }
        else
        {
            dae::Renderer::GetInstance().DrawLine(
                prevPt.x, prevPt.y,
                currPt.x, currPt.y,
                SDL_Color{ 255, 0, 0, 255 }
            );
        }

        prevPt = currPt;
    }

    dae::Renderer::GetInstance().DrawLine(
        prevPt.x, prevPt.y,
        firstPt.x, firstPt.y,
        SDL_Color{ 255, 0, 0, 255 }
    );
}
