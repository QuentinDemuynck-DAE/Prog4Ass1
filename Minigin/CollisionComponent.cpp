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

    float angle = owner.GetTransform()->GetGlobalRotation().x;
    bd.angle = glm::radians(angle);

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
    if (m_pBody)
    {
        m_pFixture = m_pBody->CreateFixture(&fd);
    }
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
   float deg = GetOwner().GetTransform()->GetGlobalRotation().x;

    if (m_pBody)
		m_pBody->SetTransform(b2Vec2{ pos.x + m_Offset.x, pos.y + m_Offset.y}, glm::radians(deg));
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

void dae::CollisionComponent::Render(glm::vec3 /*position*/, glm::vec2 /*scale*/)
{
    if (!m_DrawDebug || !m_pBody)
        return;

    auto& R = dae::Renderer::GetInstance();
    SDL_Color col{ 255, 0, 0, 255 };

    // Walk all fixtures on the body
    for (b2Fixture* f = m_pBody->GetFixtureList(); f; f = f->GetNext())
    {
        b2Shape::Type type = f->GetType();
        if (type == b2Shape::e_polygon)
        {
            auto* poly = static_cast<b2PolygonShape*>(f->GetShape());
            int32 count = poly->m_count;
            if (count < 2)
                continue;

            // Grab the first world-space point
            b2Vec2 first = m_pBody->GetWorldPoint(poly->m_vertices[0]);
            b2Vec2 prev = first;

            // Loop through the rest, drawing edges
            for (int32 i = 1; i < count; ++i)
            {
                b2Vec2 curr = m_pBody->GetWorldPoint(poly->m_vertices[i]);
                R.DrawLine(prev.x, prev.y, curr.x, curr.y, col);
                prev = curr;
            }

            // Close the loop
            R.DrawLine(prev.x, prev.y, first.x, first.y, col);
        }
    }
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
    if (!m_pFixture)
        return;

    b2Filter filter = m_pFixture->GetFilterData();
    filter.categoryBits = static_cast<uint16_t>(m_SelfLayers);
    filter.maskBits = static_cast<uint16_t>(m_CollidesWithLayers);
    m_pFixture->SetFilterData(filter);
}


