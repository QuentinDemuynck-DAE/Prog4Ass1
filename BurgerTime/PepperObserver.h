#pragma once
#include "Observer.h"

namespace dae
{
	class TextComponent;

	class PepperObserver : public Observer
	{
	public:
		PepperObserver(dae::TextComponent* textComponent);
		void Notify(Event event, dae::GameObject* gameObject) override;
	private:
		dae::TextComponent* m_TextComponent;
	};
}


