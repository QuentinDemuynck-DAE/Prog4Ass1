#pragma once

static const float g_fixedTimeStep{ 0.02f };

enum class KeyState
{
	None,
	Up,
	Down,
	Pressed	
};

const int g_maxControllers{ 4 };