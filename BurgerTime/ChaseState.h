#pragma once
#include "EnemyState.h"

class ChaseState : public EnemyState
{
public:
	void OnEnter(dae::GameObject&) override;
	void HandleInput(dae::GameObject& object, const Event& event) override;
private:
};

