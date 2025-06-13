#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <unordered_map>
#include "Globals.h"
#include "GamePad.h"
#include <SDL.h>


namespace dae
{
	enum class Action;
	class Command;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		// Raw key queries
		bool IsKeyDown(SDL_Keycode key) const;
		bool IsKeyPressed(SDL_Keycode key) const;
		bool IsKeyUp(SDL_Keycode key) const;

		// Action binding
		void BindKey(Action action, SDL_Keycode key);
		void UnbindKey(Action action);
		bool IsActionDown(Action action) const;
		bool IsActionPressed(Action action) const;
		bool IsActionUp(Action action) const;

		bool IsAction(Action action, KeyState state) const;

		void AddGamePad(std::unique_ptr<GamePad> gampad);
		GamePad* GetGamePadAtIndex(int idx) const;

	private:


		void UpdateStates();

		std::vector<std::unique_ptr<GamePad>> m_GamePads;

		std::unordered_map<SDL_Keycode, bool> m_CurrentKey;
		std::unordered_map<SDL_Keycode, bool> m_PreviousKey;
		std::unordered_map<Action, SDL_Keycode> m_ActionMap;
	};

}
