#pragma once
#include "DerivedSoundSystems.h"
#include <memory>

class SoundSystem;
class ServiceLocator final
{
public:
	static void ProvideSoundSystem(std::unique_ptr<SoundSystem>&& system) 
	{ 
		if (system.get() == nullptr)
		{
			m_sSoundSystem = std::make_unique<NullSoundSystem>();
		}
		else
		{
			m_sSoundSystem = std::move(system);
		}
	}

	static SoundSystem& GetSoundSystem() { return *m_sSoundSystem; }
private:
	static std::unique_ptr<SoundSystem> m_sSoundSystem;
};
