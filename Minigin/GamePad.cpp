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

            m_PreviousButtonState[button] = buttonPressed;
        }
    }

    bool GamePad::CheckInput(Button button) const
    {
       return m_pImpl->CheckInput(button);
    }


    int GamePad::GetId() const
    {
        return m_pImpl->GetId();
    }
}



