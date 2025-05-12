#pragma once
#include "EnemyState.h"

class EnemyComponent;

class StunnedState : public EnemyState
{
public:
	void OnEnter(dae::GameObject&) override;
	void OnExit(dae::GameObject&) override;
	void HandleInput(dae::GameObject& object, const Event& event) override;
private:
	EnemyComponent* m_EnemyComponent;
};

