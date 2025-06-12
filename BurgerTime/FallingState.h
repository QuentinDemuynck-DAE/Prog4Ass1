#pragma once
#include "IngredientState.h"

class RigidbodyComponent;

namespace dae
{
	class IngredientComponent;

	class FallingState final : public dae::IngredientState
	{
	public:
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void OnEnter(dae::GameObject&) override;
		void OnExit(dae::GameObject&) override;
		void Update(dae::GameObject&, float) override;
	private:
		const glm::vec2 m_Speed{ 0.0, 10 };
		RigidbodyComponent* m_Rigidbody;
		IngredientComponent* m_pIngredientComponent;
		float m_Timer{ 0.0f };
		const float CHECK_TIME = 0.3f;

	};
}


