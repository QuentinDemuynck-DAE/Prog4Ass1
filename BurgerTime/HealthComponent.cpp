#include "HealthComponent.h"
#include <algorithm>
#include "GameObject.h"
#include "Subject.h"
#include "Globals.h"


HealthComponent::HealthComponent(dae::GameObject& owner, int maxHealth) : Component(owner), m_Health(maxHealth), m_MaxHealth(maxHealth)
{
}

void HealthComponent::DoDamage(int amount)
{
	m_Health = std::max(0, std::min(m_Health - amount, m_MaxHealth));

	Event e{ make_sdbm_hash("do_damage") };

	e.AddArg(amount); // damage taken
	e.AddArg(m_Health); // current health

	GetOwner().GetSubject()->Notify(e);
}
