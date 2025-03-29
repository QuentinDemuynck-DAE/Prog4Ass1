#pragma once
#include "Component.h"

class ScoreComponent : public Component
{
public:
	ScoreComponent(dae::GameObject& owner);
	virtual ~ScoreComponent() = default;

	void AddScore(int amount);

private:
	int m_Score{0};
};

