#pragma once
#include "Controller.h"
#include "GamePad.h"

namespace dae {
    class GamePadControllerComponent final: public ControllerComponent {
    public:
        GamePadControllerComponent(GameObject& owner, GamePad* pad) :ControllerComponent(owner), m_GamePad(pad)
        {
            BindDefaults();
        }

    protected:
        bool CheckAction(Action action, KeyState keystate) override
        {
            if (!m_GamePad->IsConnected())
                return false;

            auto it = m_ButtonMap.find(action);
            if (it == m_ButtonMap.end())
                return false;

            return m_GamePad->CheckInput(it->second, keystate);
        }

    private:

        void BindDefaults() // I could make functions to change the keybinds at runtime now very easily
        {
            static constexpr std::pair<Action, GamePad::Button> defaults[] =
            {
                { Action::Left,  GamePad::Button::DPadLeft  },
                { Action::Right, GamePad::Button::DPadRight },
                { Action::Up,    GamePad::Button::DPadUp    },
                { Action::Down,  GamePad::Button::DPadDown  },
                {Action::Debug, GamePad::Button::LeftTrigger},
				{Action::Shoot, GamePad::Button::ButtonA}
            };

            for (auto const& [act, btn] : defaults)
            {
                m_ButtonMap[act] = btn;
                m_GamePad->AddButton(btn);
            }
        }

        GamePad* m_GamePad;
        std::unordered_map<Action, GamePad::Button> m_ButtonMap;
    };
}
