#pragma once
#include "EnemyState.h"

class EnemyComponent;

class PatrolState : public EnemyState
{
public:
    void OnEnter(dae::GameObject&) override;
    void OnExit(dae::GameObject&) override;
    void HandleInput(dae::GameObject& object, const Event& event) override;
    void Update(dae::GameObject& gameObject, float) override;
private:
    EnemyComponent* m_EnemyComponent;
};

