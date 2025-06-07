#include "DebugPositionCommand.h"

#include "GameObject.h"

dae::DebugPositionCommand::DebugPositionCommand(dae::GameObject* gameObject)
	:m_gameObj(gameObject)
{
}

void dae::DebugPositionCommand::Execute()
{
	std::cout << "PlayerPosition: " << m_gameObj->GetTransform()->GetGlobalPosition().x << ", " << m_gameObj->GetTransform()->GetGlobalPosition().y << std::endl;
}
