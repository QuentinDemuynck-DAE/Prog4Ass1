#include "PepperObserver.h"
#include "TextComponent.h"
#include <string>

#include "Globals.h"

dae::PepperObserver::PepperObserver(dae::TextComponent* textComponent)
	:m_TextComponent(textComponent)
{
}

void dae::PepperObserver::Notify(Event event, dae::GameObject*)
{
	if (event.id == make_sdbm_hash("pepper_shot"))
	{
		if (event.numberArgs >= 1)  // Ensure enough arguments exist
		{
			if (std::holds_alternative<int>(event.args[0]))
			{
				//int livesLost = std::get<int>(event.args[0]);
				int peppers = std::get<int>(event.args[0]);

				if (m_TextComponent != nullptr)
				{
					std::string text = "Peppers: " + std::to_string(peppers);
					m_TextComponent->SetText(text);
				}
			}
		}
	}
}
