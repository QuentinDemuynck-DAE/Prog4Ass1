#pragma once
#include "SoundSystem.h"

class NullSoundSystem : public SoundSystem
{
public:
	NullSoundSystem() = default;
	NullSoundSystem(const NullSoundSystem&) = delete;
	NullSoundSystem(NullSoundSystem&&) = delete;
	NullSoundSystem& operator=(const NullSoundSystem&) = delete;
	NullSoundSystem& operator=(NullSoundSystem&&) = delete;
	~NullSoundSystem() override = default;
	SoundID SoundToId(const std::string&) override
	{
		return 0;
	}
	void PlaySound(SoundID, int) override
	{

	}
};

class SDL2MixerSoundSystem : public SoundSystem
{
};

class DebugSoundSystem : public SoundSystem
{
};
