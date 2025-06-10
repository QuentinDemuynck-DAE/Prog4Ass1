#pragma once
#include "Component.h"
#include "EnemyState.h"

namespace dae
{
	class GameObject;

	class EnemyComponent : public Component
	{
	public:

		EnemyComponent(dae::GameObject& owner, std::vector<dae::GameObject*> players);
		virtual ~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;
		void Update(float deltaTime) override;
		void HandleInput(const Event event);
		void SetState(std::unique_ptr<EnemyState> newState);

		void SeekPlayer();
		void ChasePlayer();
		void SetCanMove(bool canMove);
		void Patrol(float dt);
		void ResetPatrolTimer();

	private:
		void Move(float deltaTime);
		bool m_CanMove = true;
		const float SEEK_DISTANCE = 100.0f;
		const float MOVEMENT_SPEED = 10.0f;
		const float CALCULATE_NEW_PATROL_DIR = 3.0f;
		std::unique_ptr<EnemyState> m_CurrentState;
		std::vector<dae::GameObject*> m_Players;
		dae::GameObject* m_ChasingPlayer;
		glm::vec3 m_Direction;
		float m_PatrolDirectionTimer;
	};
}


