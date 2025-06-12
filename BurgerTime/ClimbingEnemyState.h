#pragma once
#include "EnemyState.h"
#include "MapWalkerComponent.h"


namespace dae
{
	class ControllerComponent;
	class EnemyComponent;

    class ClimbingEnemyState : public EnemyState
    {
    public:
        void OnEnter(dae::GameObject&) override;
        void OnExit(dae::GameObject&) override;
        void HandleInput(dae::GameObject& object, const Event& event) override;
        void Update(dae::GameObject& gameObject, float) override;
    private:
        float m_InitialDirection{ 1.0f };
        EnemyComponent* m_EnemyComponent;
        ControllerComponent* m_EnemyController;
        std::vector<GameObject*> m_Players;
        MapWalkerComponent* m_MapWalker;
        MapWalkerComponent::ClimbDirection m_ClimbDirection;
        const float MIN_CLIMB_TIME{ 0.3f };
        float m_Timer{};
    };
}


