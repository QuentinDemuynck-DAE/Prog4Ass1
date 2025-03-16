#include <SDL.h>
#include "InputManager.h"
#include "Command.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
		// etc...
	}

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    for (auto& [key, bindings] : m_Commands)
    {
        bool keyPressed = state[SDL_GetScancodeFromKey(key)];
        bool wasPressed = m_PreviousKeyState.contains(key) ? m_PreviousKeyState[key] : false;

        if (bindings.first)
        {
            if (
                (bindings.second == KeyState::Pressed  && keyPressed) ||
                (bindings.second == KeyState::Down && keyPressed && !wasPressed) ||
                (bindings.second == KeyState::Up && wasPressed && !keyPressed)
                )
            {
                bindings.first->Execute();
            }
        }

        m_PreviousKeyState[key] = keyPressed;
    }

	for (auto& [id, gamePad] : m_GamePads)
	{
		if (gamePad->IsConnected())
		{
			gamePad->ProcessInput();
		}
	}
	return true;
}

void dae::InputManager::BindCommand(SDL_Keycode keycode, std::shared_ptr<Command> command, KeyState keyState)
{
	m_Commands.emplace(keycode, std::make_pair(command, keyState));
}

bool dae::InputManager::ConnectGamePad(std::unique_ptr<GamePad> gamePad)
{
	if (m_GamePads.contains(gamePad.get()->GetId()))
	{
		return false;
	}

	m_GamePads[gamePad.get()->GetId()] = std::move(gamePad);
    return true;
}
