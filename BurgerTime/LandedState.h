#pragma once
#include "IngredientState.h"

namespace dae
{
	class IngredientComponent;
}

namespace dae
{
	class LandedState : public dae::IngredientState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void OnExit(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;

	private:
		int numberOfActivations{};
		IngredientComponent* m_pIngredientComponent;
	};

}

