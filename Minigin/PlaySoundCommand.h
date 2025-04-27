#pragma once
#include "Command.h"
#include "ServiceLocator.h"

namespace dae
{
	class PlaySoundCommand final : public dae::Command
	{
	public:
		PlaySoundCommand(const std::string& soundPath, int volume = 128)
			: m_Volume(volume)
		{
			auto& soundSystem = ServiceLocator::GetSoundSystem();
			m_SoundID = soundSystem.SoundToId(soundPath);
		}
		void Execute() override
		{
			auto& soundSystem = ServiceLocator::GetSoundSystem();
			soundSystem.PlaySound(m_SoundID, m_Volume);
		}
	private:
		unsigned int m_SoundID = 0;
		int m_Volume;
	};
}


