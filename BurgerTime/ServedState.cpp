#include "ServedState.h"

void dae::ServedState::OnEnter(dae::GameObject& game_object)
{
	IngredientState::OnEnter(game_object);
}

void dae::ServedState::HandleInput(dae::GameObject&, const Event&)
{
}
