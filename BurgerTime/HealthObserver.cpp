#include "HealthObserver.h"
#include "Globals.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LivesComponent.h"

HealthObserver::HealthObserver(dae::TextComponent* textComponent) : m_TextComponent(textComponent)
{

}

void HealthObserver::Notify(Event event, dae::GameObject* gameObject)
{
	switch (event.id)
	{
	case make_sdbm_hash("do_damage"):
		if (event.numberArgs >= 2)  // Ensure enough arguments exist
		{
			if (std::holds_alternative<int>(event.args[0]) && std::holds_alternative<int>(event.args[1]))
			{
				//int damage = std::get<int>(event.args[0]);
				int health = std::get<int>(event.args[1]);

				if (health <= 0 && gameObject->HasComponent<LivesComponent>())
				{
					gameObject->GetComponent<LivesComponent>()->LoseLive();
				}

			}
		}
		break;
	case make_sdbm_hash("lose_live"):
		if (event.numberArgs >= 2)  // Ensure enough arguments exist
		{
			if (std::holds_alternative<int>(event.args[0]) && std::holds_alternative<int>(event.args[1]))
			{
				//int livesLost = std::get<int>(event.args[0]);
				int lives = std::get<int>(event.args[1]);

				if (m_TextComponent != nullptr)
				{
					std::string text = "# of lifes: " + std::to_string(lives);
					m_TextComponent->SetText(text);
				}
			}
		}
		break;
	default:
		break;
	}
}
