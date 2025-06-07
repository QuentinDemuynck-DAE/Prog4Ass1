#pragma once
#include "Command.h"

class PlayerComponent;

namespace dae
{
	class DebugPositionCommand : public Command
	{
	public:
		DebugPositionCommand(dae::GameObject* gameObject);
		void Execute() override;
	private:
		dae::GameObject* m_gameObj;
	};
}


