#pragma once
#include "EnemyState.h"

class ChaseState : public EnemyState
{
public:
	void HandleInput(dae::GameObject& object, const Event& event) override;
private:
};

