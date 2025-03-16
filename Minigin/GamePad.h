#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "Command.h"
#include <memory>
#include "Globals.h"
#include <unordered_map>

class GamePad
{
public:
	GamePad(unsigned long id);
	~GamePad();


	enum class Button
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		LeftShoulder,
		RightShoulder,
		Back,
		Start,
		LeftThumb,
		RightThumb,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		LeftTrigger,
		RightTrigger,
		Count
	};

	bool IsConnected() const;
	void ProcessInput();

	void BindCommand(Button keycode, std::shared_ptr<dae::Command> command, KeyState keyState);

	int GetId() const;
private:
	class impl;
	std::unique_ptr<impl> m_pImpl;
	std::unordered_map<Button, std::pair<KeyState, std::shared_ptr<dae::Command>>> m_Keys;
	std::unordered_map<Button, bool> m_PreviousButtonState;
};


