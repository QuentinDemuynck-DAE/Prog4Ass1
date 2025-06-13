#pragma once
#include "CollisionComponent.h"
#include "EnemyState.h"



namespace dae
{
	class EnemyComponent;

	class StunnedState : public EnemyState
	{
	public:
		StunnedState(bool wasWalking);
		void OnEnter(dae::GameObject&) override;
		void OnExit(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void Update(GameObject&, float) override;

	private:
		EnemyComponent* m_EnemyComponent;
		CollisionComponent* m_CollisionComponent;;
		bool m_WasWalking;
		float m_Timer{ 0.0f };
		const float MAX_STUN_DURATION{ 5.0f };
	};
}
