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

		bool CheckInput(Button button) const;

		int GetId() const;
	private:

		struct Pending {
			enum Type { Bind, Unbind }     type;
			Button           button;
			KeyState         state;        // only used for binds
			std::shared_ptr<Command> cmd;  // only used for binds
		};


		class impl;
		std::unique_ptr<impl> m_pImpl;
		std::unordered_map<Button, KeyState> m_Keys;
		std::unordered_map<Button, bool> m_PreviousButtonState;
	};
}



