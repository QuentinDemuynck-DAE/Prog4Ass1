#include "Windows.h"
#include "Xinput.h"
#include "GamePad.h"

class GamePad::impl
{
public:
	impl(unsigned long id) : m_Id(id) {}

	bool IsConnected() const
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(m_Id, &state);
		return dwResult == ERROR_SUCCESS;
	}

    bool CheckInput(GamePad::Button button)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if (XInputGetState(m_Id, &state) != ERROR_SUCCESS)
        {
            return false;
        }

        switch (button)
        {
        case GamePad::Button::ButtonA:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_A;
        case GamePad::Button::ButtonB:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_B;
        case GamePad::Button::ButtonX:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_X;
        case GamePad::Button::ButtonY:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
        case GamePad::Button::LeftShoulder:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        case GamePad::Button::RightShoulder:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        case GamePad::Button::Back:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
        case GamePad::Button::Start:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_START;
        case GamePad::Button::LeftThumb:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
        case GamePad::Button::RightThumb:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
        case GamePad::Button::DPadUp:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
        case GamePad::Button::DPadDown:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
        case GamePad::Button::DPadLeft:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
        case GamePad::Button::DPadRight:
            return state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
        case GamePad::Button::LeftTrigger:
            return state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        case GamePad::Button::RightTrigger:
            return state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        default:
            return false;
        }
    }

	unsigned long GetId() const { return m_Id; }

private:
	DWORD m_Id{};
};