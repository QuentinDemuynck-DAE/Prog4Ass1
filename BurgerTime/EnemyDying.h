#pragma once
#include "EnemyComponent.h"
#include "EnemyState.h"
#include "MapWalkerComponent.h"

namespace dae
{
	class EnemyDying : public EnemyState
	{
	public:
		void HandleInput(GameObject& object, const Event& event) override;
		void OnEnter(GameObject&) override;
		void OnExit(GameObject&) override;
		void Update(GameObject&, float) override;
	private:
		EnemyComponent* m_EnemyComponent;
		MapWalkerComponent* m_MapWalker;
		const float RESPAWN_TIME{ 6.0f };
		float m_Timer{ 0.0f };
	};
}


