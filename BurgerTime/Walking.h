#pragma once
#include "Events.h"
#include "PlayerState.h"

class PlayerComponent;

namespace dae
{
	class GameObject;
	class RigidbodyComponent;
	class MapWalkerComponent;
	class ControllerComponent;



	class Walking : public PlayerState
	{
	public:
		void OnEnter(dae::GameObject&) override;
		void HandleInput(dae::GameObject& object, const Event& event) override;

		void Update(GameObject&, float) override;

		void OnExit(GameObject&) override;

	private:
		PlayerComponent* m_PlayerComponent;
		RigidbodyComponent* m_Rigidbody;
		MapWalkerComponent* m_MapWalker;
		ControllerComponent* m_PlayerController;
	};
}


