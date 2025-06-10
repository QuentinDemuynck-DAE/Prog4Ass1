#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "Command.h"
#include <memory>
#include "Globals.h"
#include <unordered_map>

namespace dae
{
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
		void UnbindCommand(Button keycode);

		int GetId() const;
	private:

		struct Pending {
			enum Type { Bind, Unbind }     type;
			Button           button;
			KeyState         state;        // only used for binds
			std::shared_ptr<Command> cmd;  // only used for binds
		};

		std::vector<Pending> m_Pending;

		class impl;
		std::unique_ptr<impl> m_pImpl;
		std::unordered_map<Button, std::pair<KeyState, std::shared_ptr<dae::Command>>> m_Keys;
		std::unordered_map<Button, bool> m_PreviousButtonState;
	};
}



