#include "LivesComponent.h"
#include <algorithm>
#include "GameObject.h"
#include "Subject.h"
#include "Events.h"
#include "Globals.h"

void LivesComponent::Update(float)
{
	if(m_TextComponent)
		m_TextComponent->SetText("Lives: " + m_Lives);
}

LivesComponent::LivesComponent(dae::GameObject& owner, int maxLives) : m_Lives(maxLives), m_MaxLives(maxLives), Component(owner)
{
	if (owner.HasComponent<dae::TextComponent>())
	{
		m_TextComponent = owner.GetComponent<dae::TextComponent>();
	}
}

void LivesComponent::LoseLive(int amount)
{
	m_Lives = std::max(0, std::min(m_Lives - amount, m_MaxLives));

	Event e{ make_sdbm_hash("lose_live") };

	e.AddArg(amount); // lives lost
	e.AddArg(m_Lives); // Current lives

	GetOwner().GetSubject()->Notify(e);
}
