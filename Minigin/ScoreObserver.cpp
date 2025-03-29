#include "ScoreObserver.h"
#include "Globals.h"
#include "SteamAchievement.h"

ScoreObserver::ScoreObserver(dae::TextComponent* textComponent)
	: m_TextComponent(textComponent)
{
}

void ScoreObserver::Notify(Event event, dae::GameObject*)
{
	switch (event.id)
	{
	case make_sdbm_hash("add_score"):
		if (event.numberArgs >= 2)  // Ensure enough arguments exist
		{
			if (std::holds_alternative<int>(event.args[0]) && std::holds_alternative<int>(event.args[1]))
			{
				int score = std::get<int>(event.args[1]);

				if (m_TextComponent != nullptr)
				{
					std::string text = "Score: " + std::to_string(score);
					m_TextComponent->SetText(text);
				}

				if (score >= 500)
				{
					SteamAchievement::GetInstance().SetAchievement("ACH_WIN_ONE_GAME");
				}
			}
		}
		break;
	default:
		break;
	}
}
