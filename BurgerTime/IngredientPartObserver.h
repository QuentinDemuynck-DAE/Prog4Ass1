#pragma once
#include "Observer.h"

namespace dae
{
	class IngredientPartObserver : public Observer
	{
	public:
		IngredientPartObserver(GameObject* pIngredientObject);
		void Notify(Event event, dae::GameObject* gameObject) override;
	private:
		GameObject* m_pIngredientObject;
	};
}

