#include "GamePad.h"
#include "XInputImplGamePad.cpp"

namespace dae
{
    GamePad::GamePad(unsigned long id)
    {
        m_pImpl = std::make_unique<impl>(id);
        for (auto b : { Button::DPadLeft, Button::DPadRight, Button::DPadUp, Button::DPadDown /*, …*/ })
            m_CurrentKey[b] = false;

    }

    GamePad::~GamePad() = default;

    void GamePad::AddButton(Button button)
    {
    	m_CurrentKey[button] = false;
    	m_PreviousButtonState[button] = false;
    }

    void GamePad::RemoveButton(Button button)
    {
         // erase from the current state map
         auto itKey = m_CurrentKey.find(button);
         if (itKey != m_CurrentKey.end())
             m_CurrentKey.erase(itKey);

         // erase from the previous state map
         auto itPrev = m_PreviousButtonState.find(button);
         if (itPrev != m_PreviousButtonState.end())
             m_PreviousButtonState.erase(itPrev);
    }

    bool GamePad::IsConnected() const
    {
        return m_pImpl->IsConnected();
    }

    void GamePad::ProcessInput()
    {
        for (auto& [button, keyState] : m_CurrentKey)
        {
            bool isPressed = m_pImpl->CheckInput(button);

            m_PreviousButtonState[button] = keyState;
            keyState = isPressed;
        }
    }

    bool GamePad::CheckInput(Button button, KeyState state) const
    {
        auto itCurrent = m_CurrentKey.find(button);
        auto itPrevious = m_PreviousButtonState.find(button);
        if (itCurrent == m_CurrentKey.end() || itPrevious == m_PreviousButtonState.end())
            return false;

        bool current = itCurrent->second;
        bool previous = itPrevious->second;

        switch (state)
        {
        case KeyState::Pressed:  return current;          // held
        case KeyState::Down:     return current && !previous; // just pressed
        case KeyState::Up:       return !current && previous; // just released
        }
        return false;
    }


    int GamePad::GetId() const
    {
        return m_pImpl->GetId();
    }
}



