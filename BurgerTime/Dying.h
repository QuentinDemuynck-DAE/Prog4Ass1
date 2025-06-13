#pragma once
#include "Events.h"
#include "LivesComponent.h"
#include "MapWalkerComponent.h"
#include "PlayerState.h"
class PlayerComponent;

namespace dae
{
	class GameObject;

	class Dying : public PlayerState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;
		void Update(dae::GameObject&, float) override;
		void OnExit(GameObject&) override;

	private:
		PlayerComponent* m_PlayerComponent;
		LivesComponent* m_LivesComponent;
		MapWalkerComponent* m_MapWalkerCommponent;
		float m_Timer{ 0.0f };
		const float RESPAWN_TIME{3.0f};
	};
}


