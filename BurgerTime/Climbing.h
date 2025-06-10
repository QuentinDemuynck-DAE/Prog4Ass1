#pragma once
#include "Events.h"
#include "PlayerState.h"
class PlayerComponent;

namespace dae
{
	class GameObject;

	class Climbing :public PlayerState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;

		void OnExit(GameObject&) override;
	private:
		PlayerComponent* m_PlayerComponent;
	};
}


