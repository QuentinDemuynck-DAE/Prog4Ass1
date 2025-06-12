#pragma once
#pragma once
#include "Controller.h"
#include "GamePad.h"

namespace dae {
    class GamePadControllerComponent final: public ControllerComponent {
    public:
        GamePadControllerComponent(GameObject& owner, GamePad* pad) :ControllerComponent(owner), m_GamePad(pad) {}

    protected:
        bool CheckAction(Action action) override
        {
            if (!m_GamePad->IsConnected())
                return false;

            using Button = GamePad::Button;
            switch (action)
            {
            case Action::Left:  return m_GamePad->CheckInput(Button::DPadLeft);
            case Action::Right: return m_GamePad->CheckInput(Button::DPadRight);
            case Action::Up:      return m_GamePad->CheckInput(Button::DPadUp);
            case Action::Down:    return m_GamePad->CheckInput(Button::DPadDown);
            default:                return false;
            }
        }

    private:
        GamePad* m_GamePad;
    };
}
