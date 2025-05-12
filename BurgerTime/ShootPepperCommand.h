#pragma once
#include "Command.h"

class PlayerComponent;

namespace dae
{
	class ShootPepperCommand : public Command
	{
	public:
		ShootPepperCommand(dae::GameObject* gameObject);
		void Execute() override;
	private:
		PlayerComponent* m_PlayerComponent;
		dae::GameObject* m_OwningObj;
	};
}


