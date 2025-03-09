#include "GamePad.h"
#include "XInputImplGamePad.cpp"


GamePad::GamePad(unsigned long id)
{
	m_pImpl = std::make_unique<impl>(id);
}

GamePad::~GamePad() = default;

bool GamePad::IsConnected() const
{
	return m_pImpl->IsConnected();
}

void GamePad::ProcessInput()
{
    for (auto& [button, keyState] : m_Keys)
    {
        bool buttonPressed = m_pImpl->CheckInput(button);
        bool wasPressed = m_PreviousButtonState.contains(button) ? m_PreviousButtonState[button] : false;

        if (
            (keyState.first == KeyState::Pressed && buttonPressed) ||
            (keyState.first == KeyState::Down && buttonPressed && !wasPressed) ||
            (keyState.first == KeyState::Up && wasPressed && !buttonPressed)
            )
        {
			keyState.second->Execute();
        }

        m_PreviousButtonState[button] = buttonPressed;
    }
}

void GamePad::BindCommand(Button keycode, std::shared_ptr<Command> command, KeyState keyState)
{
	m_Keys[keycode] = std::make_pair(keyState, command);
}

int GamePad::GetId() const
{
    return m_pImpl->GetId();
}
