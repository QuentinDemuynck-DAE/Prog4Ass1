#pragma once
#include "Component.h"
class RotateComponent final : public Component
{
public:
	RotateComponent(dae::GameObject& owner, const float speed, bool clockwise = false);

	void Update(float deltaTime) override;
	void SetSpeed(const float speed) { m_speed = speed; }
	float GetSpeed() const { return m_speed; }

	bool GetIsClockwise() const { return m_clockwise; }
	void SetIsClockwise(bool clockwise) { m_clockwise = clockwise; }
	
private:
	float m_speed{ 10 };
	bool m_clockwise{ true };
};

