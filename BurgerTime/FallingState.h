#pragma once
#include "IngredientState.h"

namespace dae
{
	class FallingState : public dae::IngredientState
	{
	public:
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void OnEnter(dae::GameObject&) override;
		void OnExit(dae::GameObject&) override;
		void Update(dae::GameObject&, float) override;
	};
}


