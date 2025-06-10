#pragma once
#include "Events.h"
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
	private:
		PlayerComponent* m_EnemyComponent;
	};
}


