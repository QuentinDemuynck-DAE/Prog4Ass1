#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <unordered_map>
#include "Globals.h"
#include "GamePad.h"


namespace dae
{
	class Command;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(SDL_Keycode keycode, std::shared_ptr<Command> command, KeyState keyState);
		bool ConnectGamePad(std::unique_ptr<GamePad> gamePad);

	private:
		std::unordered_map<SDL_Keycode, std::pair<std::shared_ptr<Command> , KeyState>> m_Commands;
		std::unordered_map<SDL_Keycode, bool> m_PreviousKeyState;
		std::unordered_map<int, std::unique_ptr<GamePad>> m_GamePads;
	};

}
