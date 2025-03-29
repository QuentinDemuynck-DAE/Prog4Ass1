#include "ScoreComponent.h"
#include "Events.h"
#include "Globals.h"
#include "GameObject.h"
#include "Subject.h"

ScoreComponent::ScoreComponent(dae::GameObject& owner)
	:Component(owner)
{
}

void ScoreComponent::AddScore(int amount)
{
	m_Score += amount;
	Event e{ make_sdbm_hash("add_score") };

	e.AddArg(amount); // Score gained
	e.AddArg(m_Score); // Current score

	GetOwner().GetSubject()->Notify(e);
}
