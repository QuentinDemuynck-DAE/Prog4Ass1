#pragma once
#include "Collision.h"
#include "CollisionComponent.h"
#include "Scene.h"
#include "SceneManager.h"

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
        auto* compA = reinterpret_cast<dae::CollisionComponent*>(bodyA->GetUserData());
        auto* compB = reinterpret_cast<dae::CollisionComponent*>(bodyB->GetUserData());

        if (!compA || !compB)
            return;

        if (!dae::SceneManager::GetInstance().GetActiveScene().get()->Contains(&compA->GetOwner()) || !dae::SceneManager::GetInstance().GetActiveScene().get()->Contains(&compB->GetOwner()))
            return;

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
