#pragma once




namespace dae {
	struct SteamAchievement {
		int Id;
		const char* chID;
		char name[128];
		char description[256];
		bool achieved;
		int iconImage;
	};

	enum class CollisionLayers : uint16_t // max 16
	{
		MAP = 1 << 0,  
		PLAYER = 1 << 1,  
		ENEMY = 1 << 2, 
		SALT = 1 << 3, 
		INGREDIENT = 1 << 4,  
		PLATE = 1 << 5,
		MAPWALKER = 1 << 6,


		ALL = MAP | PLAYER | ENEMY | SALT | INGREDIENT | PLATE | MAPWALKER,
		NONE = 0
	};

	// operator overloading for the collision layers;
	using CL = dae::CollisionLayers;

	inline CL operator|(CL a, CL b) {
		return static_cast<CL>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
	}

	inline CL operator&(CL a, CL b) {
		return static_cast<CL>(static_cast<uint16_t>(a) & static_cast<uint16_t>(b));
	}

	inline CL operator~(CL a) {
		return static_cast<CL>(~static_cast<uint16_t>(a));
	}

	inline CL operator-(CL a, CL b) {
		return static_cast<CL>(static_cast<uint16_t>(a) & ~static_cast<uint16_t>(b));
	}

	inline bool operator!(CL a) {
		return static_cast<uint16_t>(a) == 0;
	}

}

static const float g_fixedTimeStep{ 0.02f };
static const int g_windowWidth{640}; // map is 240 x 240
static const int g_windowHeight{480};

// Keystate
enum class KeyState
{
	None,
	Up,
	Down,
	Pressed	
};

static const int g_maxControllers{ 2 };

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
