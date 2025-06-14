#pragma once
#include "InputManager.h"
#include <SDL.h>
#include "Controller.h"
namespace dae
{
     class KeyboardControllerComponent : public ControllerComponent {
     public:
         KeyboardControllerComponent(GameObject& owner) : ControllerComponent(owner)
         {
             BindDefaults();
         }

     protected:
         bool CheckAction(Action action, KeyState keystate) override
         {
             if (!m_Binded)
                 return false;

             auto& input = InputManager::GetInstance();
             return input.IsAction(action, keystate);
         }
     private:
         void BindDefaults()
         {
             static constexpr std::pair<Action, SDL_Keycode> defaults[] = {
                 { Action::Left,  SDLK_a },
                 { Action::Right, SDLK_d },
                 { Action::Up,    SDLK_w },
                 { Action::Down,  SDLK_s },
				{Action::Shoot, SDLK_l}
             };

             auto& input = InputManager::GetInstance();
             for (auto const& [act, key] : defaults)
             {
                 input.BindKey(act, key);
             }
             m_Binded = true;
         }

         bool m_Binded{ false };
     };
}

