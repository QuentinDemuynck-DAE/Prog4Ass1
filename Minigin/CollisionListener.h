#pragma once
#include "Collision.h"
#include "CollisionComponent.h"

class CollisionListener : public b2ContactListener
{
public:
    // Called when two fixtures start overlapping
    void BeginContact(b2Contact* contact) override
    {
        dispatch(contact, true);
    }

    // Called when they stop overlapping
    void EndContact(b2Contact* contact) override
    {
        dispatch(contact, false);
    }

private:
    void dispatch(b2Contact* contact, bool enter)
    {
        auto* fixA = contact->GetFixtureA();
        auto* fixB = contact->GetFixtureB();

        // Get the two bodies
        auto* bodyA = fixA->GetBody();
        auto* bodyB = fixB->GetBody();

        // Extract our CollisionComponent pointer from userData
        auto* compA = reinterpret_cast<CollisionComponent*>(bodyA->GetUserData());
        auto* compB = reinterpret_cast<CollisionComponent*>(bodyB->GetUserData());

        if (compA && compB)
        {
            if (enter)
            {
                compA->OnTriggerEnter(compB);
                compB->OnTriggerEnter(compA);
            }
            else
            {
                compA->OnTriggerExit(compB);
                compB->OnTriggerExit(compA);
            }
        }
    }
};
