#include "ScoreObserver.h"
#include "Globals.h"

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
			}
		}
		break;
	default:
		break;
	}
}
