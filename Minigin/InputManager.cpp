#include <SDL.h>
#include "InputManager.h"
#include "Command.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	UpdateStates();

	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}

		if (e.type == SDL_KEYDOWN)
			m_CurrentKey[e.key.keysym.sym] = true;
		else if (e.type == SDL_KEYUP)
			m_CurrentKey[e.key.keysym.sym] = false;
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& gamepad: m_GamePads)
	{
		gamepad.get()->ProcessInput();
	}

	return true;
}

void dae::InputManager::UpdateStates()
{
	m_PreviousKey = m_CurrentKey;
}

bool dae::InputManager::IsKeyDown(SDL_Keycode key) const
{
	auto it = m_CurrentKey.find(key);
	return it != m_CurrentKey.end() && it->second;
}

bool dae::InputManager::IsKeyPressed(SDL_Keycode key) const
{
	auto itC = m_CurrentKey.find(key);
	auto itP = m_PreviousKey.find(key);
	bool curr = (itC != m_CurrentKey.end() && itC->second);
	bool prev = (itP != m_PreviousKey.end() && itP->second);
	return curr && !prev;
}

bool dae::InputManager::IsKeyUp(SDL_Keycode key) const
{
	auto itC = m_CurrentKey.find(key);
	auto itP = m_PreviousKey.find(key);
	bool curr = (itC != m_CurrentKey.end() && itC->second);
	bool prev = (itP != m_PreviousKey.end() && itP->second);
	return !curr && prev;
}

void dae::InputManager::BindKey(Action action, SDL_Keycode key)
{
	m_ActionMap[action] = key;
}

void dae::InputManager::UnbindKey(Action action)
{
	m_ActionMap.erase(action);
}

bool dae::InputManager::IsActionDown(Action action) const
{
	auto it = m_ActionMap.find(action);
	return it != m_ActionMap.end() && IsKeyDown(it->second);
}

bool dae::InputManager::IsActionPressed(Action action) const
{
	auto it = m_ActionMap.find(action);
	return it != m_ActionMap.end() && IsKeyPressed(it->second);
}

bool dae::InputManager::IsActionUp(Action action) const
{
	auto it = m_ActionMap.find(action);
	return it != m_ActionMap.end() && IsKeyUp(it->second);
}

bool dae::InputManager::IsAction(Action action, KeyState state) const
{
	auto it = m_ActionMap.find(action);
	if (it == m_ActionMap.end()) return false;

	SDL_Keycode key = it->second;
	switch (state)
	{
	case KeyState::Pressed:  return IsKeyDown(key);    // holding
	case KeyState::Down:     return IsKeyPressed(key); // just pressed
	case KeyState::Up:       return IsKeyUp(key);      // just released
	}
	return false;
}

void dae::InputManager::AddGamePad(std::unique_ptr<GamePad> gampad)
{
	m_GamePads.push_back(std::move(gampad));
}
