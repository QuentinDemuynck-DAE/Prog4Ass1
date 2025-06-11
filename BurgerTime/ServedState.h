#pragma once
#include "IngredientState.h"

namespace dae
{
	class ServedState final : public dae::IngredientState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;
	};
}

