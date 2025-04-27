#pragma once
#include <string>

using SoundID = unsigned int;

class SoundSystem
{

public:
	SoundSystem() = default;
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;
	virtual ~SoundSystem() = default;

	virtual SoundID SoundToId(const std::string& filePath) = 0;
	virtual void PlaySound(SoundID soundID, int volume) = 0;
};

