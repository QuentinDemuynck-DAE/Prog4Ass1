#pragma once
#include "EnemyState.h"

class StunnedState : public EnemyState
{
public:
	void HandleInput(dae::GameObject& object, const Event& event) override;
private:
};

