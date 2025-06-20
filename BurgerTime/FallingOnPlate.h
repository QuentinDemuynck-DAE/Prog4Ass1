#pragma once
#include "IngredientState.h"

namespace dae
{
	class IngredientComponent;
	class RigidbodyComponent;

	class FallingOnPlate final : public IngredientState
	{
	public:
		void HandleInput(GameObject& object, const Event& event) override;
		void OnEnter(GameObject&) override;
		void Update(GameObject&, float) override;
		void OnExit(GameObject&) override;

	private:
		const glm::vec2 m_Speed{ 0.0, 10 };
		RigidbodyComponent* m_Rigidbody;
		IngredientComponent* m_pIngredientComponent;
		float m_Timer{ 0.0f };
		const float CHECK_TIME = 0.05f;


	};
}
