#pragma once
#include "Component.h"
#include "EnemyState.h"

namespace dae
{
	class GameObject;

	class EnemyComponent : public Component
	{
	public:

		EnemyComponent(dae::GameObject& owner, std::vector<dae::GameObject*> players, int value);
		virtual ~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;
		void Update(float deltaTime) override;
		void HandleInput(const Event event);
		void SetState(std::unique_ptr<EnemyState> newState);

		std::vector<dae::GameObject*> GetPlayers() const;
		GameObject* GetClosestPlayer() const;
		bool CanHit() const;
		void SetCanHit(const bool& canHit);
		const int& GetValue() const;

	private:
		const int m_Value{ 100 };
		bool m_CanHit{ true };
		std::unique_ptr<EnemyState> m_CurrentState;
		std::vector<dae::GameObject*> m_Players;
		dae::GameObject* m_ChasingPlayer;
		glm::vec3 m_Direction;
		float m_PatrolDirectionTimer;
	};
}


