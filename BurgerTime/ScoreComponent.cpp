#include "ScoreComponent.h"
#include "Events.h"
#include "GameManager.h"
#include "Globals.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Subject.h"

namespace dae
{
	ScoreComponent::ScoreComponent(dae::GameObject& owner, TextComponent* text)
		:Component(owner), m_TextComponent(text)
	{
	}

	void ScoreComponent::FixedUpdate()
	{
		if (m_TextComponent)
		{
			std::string text = "Score: " + std::to_string(GameManager::GetInstance().Score());
			m_TextComponent->SetText(text );
		}
	}
}

