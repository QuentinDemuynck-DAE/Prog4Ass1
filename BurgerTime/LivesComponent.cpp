#include "LivesComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "Events.h"
#include "GameManager.h"
#include "Globals.h"
#include <algorithm> // Ensure this header is included for std::min and std::max

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
   // Explicitly specify the namespace for std::min and std::max to avoid ambiguity
	m_Lives = std::clamp(m_Lives - amount, 0, m_MaxLives);

   Event e{ make_sdbm_hash("lose_live") };

   e.AddArg(amount); // lives lost
   e.AddArg(m_Lives); // Current lives

   GetOwner().GetSubject()->Notify(e);

   if (m_Lives == 0)
   {
       dae::GameManager::GetInstance().EliminatePlayer(GetOwner());
   }
}
