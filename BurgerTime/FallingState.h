#pragma once
#include "IngredientState.h"


namespace dae
{
	class IngredientComponent;
	class RigidbodyComponent;

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
		// to not instantly collide
		float m_Timer{ 0.0f };
		const float CHECK_TIME = 1.0f;

		int m_LayersToFall{ 1 };
		int m_NumEnemiesInitial{ 0 };

		constexpr int GetIngredientsTarget(int level)
		{
			if (level < 0 || level > 7)
				throw std::out_of_range{ "GetIngredientsTarget: level must be 0..7" };

			if (level == 0)
				return 0;

			return 500 << (level - 1);
		}

	};
}


