#pragma once
#include "EnemyComponent.h"
#include "EnemyState.h"
#include "MapWalkerComponent.h"
#include "RigidbodyComponent.h"

namespace dae
{
	class EnemyFallingAlong : public EnemyState
	{
	public:
		EnemyFallingAlong(GameObject* ingredient);
		void HandleInput(GameObject& object, const Event& event) override;
		void OnEnter(GameObject&) override;
		void Update(GameObject&, float) override;
		void OnExit(GameObject&) override;

	private:
		RigidbodyComponent* m_IngredientBody;
		GameObject* m_Ingredient;
		RigidbodyComponent* m_OwnBody;
		EnemyComponent* m_EnemyComponent;
		MapWalkerComponent* m_MapWalker;
	};

}

