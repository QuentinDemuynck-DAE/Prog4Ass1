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
	         
         }

     protected:
         bool CheckAction(Action action) override
         {
             auto& inputManager = InputManager::GetInstance();
             switch (action)
             {
             case Action::Left:  return inputManager.IsKeyDown(SDLK_a);
             case Action::Right: return inputManager.IsKeyDown(SDLK_d);
             case Action::Up :     return inputManager.IsKeyDown(SDLK_w);
             case Action::Down:    return inputManager.IsKeyDown(SDLK_s);
             default:                return false;
             }
         }
     };
}

