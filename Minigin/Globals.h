#pragma once

static const float g_fixedTimeStep{ 0.02f };

// Keystate
enum class KeyState
{
	None,
	Up,
	Down,
	Pressed	
};

const int g_maxControllers{ 4 };

// Hash functions

// Borrowed from the slides :), no need in remaking stuff that's already given right!
template <int length> struct sdbm_hash
{
	constexpr static unsigned int _calculate(const char* const text, unsigned int& value) {
		const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
		value = character + (value << 6) + (value << 16) - value;
		return text[length - 1];
	}
	constexpr static unsigned int calculate(const char* const text) {
		unsigned int value = 0;
		const auto character = _calculate(text, value);
		return character + (value << 6) + (value << 16) - value;
	}
};
template <> struct sdbm_hash<1> {
	constexpr static int _calculate(const char* const text, unsigned int&) { return text[0]; }
};
template <size_t N> constexpr unsigned int make_sdbm_hash(const char(&text)[N]) {
	return sdbm_hash<N - 1>::calculate(text);
}
