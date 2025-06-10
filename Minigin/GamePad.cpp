#include "GamePad.h"
#include "XInputImplGamePad.cpp"

namespace dae
{
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

        // Deffered binding and unbinding to resolve crashes
        for (auto& p : m_Pending)
        {
            if (p.type == Pending::Unbind)
                m_Keys.erase(p.button);
            else
                m_Keys[p.button] = { p.state, p.cmd };
        }
        m_Pending.clear();
    }

    void GamePad::BindCommand(Button keycode, std::shared_ptr<dae::Command> command, KeyState keyState)
    {
        m_Pending.push_back(Pending{ Pending::Bind, keycode, keyState, command });
    }

    void GamePad::UnbindCommand(Button keycode)
    {
        m_Pending.push_back(Pending{ Pending::Unbind, keycode, KeyState{}, nullptr });
    }

    int GamePad::GetId() const
    {
        return m_pImpl->GetId();
    }
}



