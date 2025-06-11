#pragma once
#include "IngredientState.h"

namespace dae
{
	class ServedState : public dae::IngredientState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void OnExit(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void Update(dae::GameObject&, float) override;
	};
}

