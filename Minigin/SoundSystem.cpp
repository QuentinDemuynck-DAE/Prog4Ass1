#include "SoundSystem.h"
#include "ServiceLocator.h"

std::unique_ptr<SoundSystem> ServiceLocator::m_sSoundSystem = std::make_unique<NullSoundSystem>();