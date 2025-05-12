#pragma once
#include "EnemyState.h"

class EnemyComponent;

class ChaseState : public EnemyState
{
public:
	void OnEnter(dae::GameObject&) override;
	void HandleInput(dae::GameObject& object, const Event& event) override;
	void Update(dae::GameObject&, float) override;
private:
	EnemyComponent* m_EnemyComponent;
};

