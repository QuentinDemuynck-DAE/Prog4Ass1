#pragma once
#include "AiController.h"
#include "EnemyState.h"
#include "MapWalkerComponent.h"

namespace dae
{
	class EnemyComponent;
	class ControllerComponent;
	class MapWalkerComponent;

	class WalkingEnemyState : public EnemyState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void Update(dae::GameObject&, float) override;
		void OnExit(GameObject&) override;

	private:

		void UpdateGoalDirection(GameObject& gameObject);

		float m_InitialDirection{ 1.0f };

		MapWalkerComponent::ClimbDirection m_GoalClimbDirection;
		EnemyComponent* m_EnemyComponent;

		AiController* m_EnemyAIController;
		std::vector<ControllerComponent*> m_Controllers;

		std::vector<GameObject*> m_Players;
		MapWalkerComponent* m_MapWalker;

		float m_Timer{ 0.0f };
		float m_TimeSinceLastWallHit{ 999.0f }; // sometimes you hit the wall twice then the direction would invert again so this way I have a small time offset
		const float WALL_HIT_GRACE{ 0.6f };
		const float MIN_WALK_TIME{ 0.3f };
	};
}


