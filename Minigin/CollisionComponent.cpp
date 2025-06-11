#include "CollisionComponent.h"

#include "Events.h"
#include "GameObject.h"
#include "Globals.h"
#include "Renderer.h"
#include "Subject.h"

dae::CollisionComponent::CollisionComponent(dae::GameObject& owner,
                                            b2World& world,
                                            dae::CollisionLayers collisionLayerSelf,
                                            dae::CollisionLayers collisionLayerToCollide,
                                            const glm::vec2& halfSize,
                                            const glm::vec2& offset,
                                            bool dynamic,
                                            bool isSensor)
    : Component(owner)
    , m_World(world)
	, m_SelfLayers(collisionLayerSelf)
	, m_CollidesWithLayers(collisionLayerToCollide)
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
         b2Vec2{ m_HalfSize.x, m_HalfSize.y }, //So I don't use offset here because this just makes sure its on the top left like any other drawing
         0.0f
    );

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.isSensor = isSensor;
    m_pFixture = m_pBody->CreateFixture(&fd);
    UpdateCollisionLayers();
    m_Offset = scaledOffset;
}


dae::CollisionComponent::~CollisionComponent()
{
    if (m_pBody)
    {
        m_World.DestroyBody(m_pBody);
    }
}

void dae::CollisionComponent::Update(float)
{
    // Snap the Box2D body to match the GameObject transform each frame
    glm::vec3 pos = GetOwner().GetTransform()->GetGlobalPosition();
    glm::vec3 eulerDeg = GetOwner().GetTransform()->GetGlobalRotation();
    float   angleRad = glm::radians(eulerDeg.z);

    m_pBody->SetTransform(b2Vec2{ pos.x + m_Offset.x, pos.y + m_Offset.y}, angleRad);
}


void dae::CollisionComponent::OnTriggerEnter(CollisionComponent* other)
{
    Event e{ make_sdbm_hash("collision_enter") };
    e.AddArg(this);
    e.AddArg(other);
    GetOwner().GetSubject()->Notify(e);
}

void dae::CollisionComponent::OnTriggerExit(CollisionComponent* other)
{

    Event e{ make_sdbm_hash("collision_exit") };
    e.AddArg(this);
    e.AddArg(other);
    GetOwner().GetSubject()->Notify(e);
}

void dae::CollisionComponent::Render(glm::vec3 position, glm::vec2)
{
    Component::Render(position, glm::vec2{ 1,1 }); // m_HalfSize gets scaled

    if (!m_DrawDebug || !m_pFixture)
        return;

    //  grab your rectangle in world space
    dae::Rectangle r = GetRectangle();

    // convert to four corner points
    glm::vec2 topLeft{ r.x,           r.y };
    glm::vec2 topRight{ r.x + r.width, r.y };
    glm::vec2 bottomRight{ r.x + r.width, r.y + r.height };
    glm::vec2 bottomLeft{ r.x,           r.y + r.height };

    // draw the four edges
    auto& R = dae::Renderer::GetInstance();
    SDL_Color col{ 255, 0, 0, 255 };

    R.DrawLine(topLeft.x, topLeft.y, topRight.x, topRight.y, col);
    R.DrawLine(topRight.x, topRight.y, bottomRight.x, bottomRight.y, col);
    R.DrawLine(bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y, col);
    R.DrawLine(bottomLeft.x, bottomLeft.y, topLeft.x, topLeft.y, col);
}

void dae::CollisionComponent::AddCollisionLayerSelf(dae::CollisionLayers layer)
{
    if (!(m_SelfLayers & layer))
    {
        m_SelfLayers = m_SelfLayers | layer;
        UpdateCollisionLayers();
    }
}

void dae::CollisionComponent::AddCollisionLayerToCollide(dae::CollisionLayers layer)
{
    if (!(m_CollidesWithLayers & layer))
    {
        m_CollidesWithLayers = m_CollidesWithLayers | layer;
        UpdateCollisionLayers();
    }
}

void dae::CollisionComponent::RemoveCollisionLayerToSelf(dae::CollisionLayers layer)
{
    m_SelfLayers = m_SelfLayers & ~layer;
    UpdateCollisionLayers();
}

void dae::CollisionComponent::RemoveCollisionLayerToCollide(dae::CollisionLayers layer)
{
    m_CollidesWithLayers = m_CollidesWithLayers & ~layer;
    UpdateCollisionLayers();
}

dae::Rectangle dae::CollisionComponent::GetRectangle() const
{
    // This is the world-space center of the box
    b2Vec2 center = m_pBody->GetPosition();

    // Build a Rectangle whose x/y are the top-left
    dae::Rectangle rect;
    rect.x = center.x;
    rect.y = center.y;
    rect.width = m_HalfSize.x * 2.0f;
    rect.height = m_HalfSize.y * 2.0f;
    return rect;
}



void dae::CollisionComponent::UpdateCollisionLayers()
{
    b2Filter filter = m_pFixture->GetFilterData();
    filter.categoryBits = static_cast<uint16_t>(m_SelfLayers);
    filter.maskBits = static_cast<uint16_t>(m_CollidesWithLayers);
    m_pFixture->SetFilterData(filter);
}


