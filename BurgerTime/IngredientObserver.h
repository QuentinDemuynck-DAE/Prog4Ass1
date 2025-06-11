#pragma once
#include "Observer.h"


namespace dae
{
	class IngredientObserver : public Observer
	{
	public:
		void Notify(Event event, dae::GameObject* gameObject) override;
	};
}

