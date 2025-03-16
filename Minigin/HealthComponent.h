#pragma once
#include "Component.h"
class HealthComponent : public Component
{
public:

	HealthComponent(dae::GameObject& owner, int maxHealth);
	virtual ~HealthComponent() = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void DoDamage(int amount = 1);

private:

	int m_Health;
	int m_MaxHealth;
};

