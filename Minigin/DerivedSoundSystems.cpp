#include "DerivedSoundSystems.h"
#include "SdlMixerImpl.cpp"
#include <iostream>                 
#include <exception>                
#include <string>                            

NoisySoundSystem::NoisySoundSystem()
	: m_pImpl(std::make_unique<Impl>()) 
	, m_worker(&NoisySoundSystem::WorkerLoop, this)
{

}

NoisySoundSystem::~NoisySoundSystem()
{
	m_running = false;
	m_queue.Stop();
	if (m_worker.joinable())
	{
		m_worker.join();
	}
}

SoundID NoisySoundSystem::SoundToId(const std::string& filePath)
{

	auto it = m_pathToID.find(filePath);
	if (it != m_pathToID.end()) {
		return it->second;
	}
	SoundID id = m_nextID++;

	m_pathToID[filePath] = id;

	m_queue.Push({ SoundEvent::Type::Load, id, filePath,0 });

	return id;
}

void NoisySoundSystem::PlaySound(SoundID id, int volume)
{
	// 1) enqueue the play
	m_queue.Push({ SoundEvent::Type::Play, id, "", volume });
}

void NoisySoundSystem::WorkerLoop() 
{
	SoundEvent event;
	while (m_running && m_queue.Pop(event)) 
	{
		try 
		{
			if (event.type == SoundEvent::Type::Load) 
			{
				auto* chunk = m_pImpl->Load(event.path);
				m_pImpl->Store(event.id, chunk);
			}
			else 
			{ // Play
				auto* chunk = m_pImpl->Get(event.id);
				if (chunk) 
				{
					m_pImpl->Play(chunk, event.volume);
				}
				else 
				{
					throw std::runtime_error("Sound ID not found: " + std::to_string(event.id));
				}			
			}
		}
		catch (const std::exception& e)
		{
			// Handle error (e.g., log it)
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
}
