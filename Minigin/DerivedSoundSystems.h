#pragma once
#include "SoundSystem.h"
#include "ThreadSafeQueue.h"
#include <memory>
#include <unordered_map>
#include <iostream>

struct SoundEvent final
{
public:
	enum class Type { Load, Play } type;
	unsigned int      id;      // for both
	std::string       path;    // only for Load
	int               volume;  // only for Play
};

class NullSoundSystem final : public SoundSystem
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

class NoisySoundSystem final: public SoundSystem
{
public:
	NoisySoundSystem();
	NoisySoundSystem(const NoisySoundSystem&) = delete;
	NoisySoundSystem(NoisySoundSystem&&) = delete;
	NoisySoundSystem& operator=(const NoisySoundSystem&) = delete;
	NoisySoundSystem& operator=(NoisySoundSystem&&) = delete;
	~NoisySoundSystem() override;
	
	SoundID SoundToId(const std::string& filePath) override;
	void PlaySound(SoundID id, int volume) override;
private:
	class Impl;
	void           WorkerLoop();
	std::unique_ptr<Impl>    m_pImpl;

	ThreadSafeQueue<SoundEvent> m_queue;
	std::thread                 m_worker;
	std::atomic<bool>           m_running{ true };

	// for generating new SoundIDs
	std::atomic<SoundID>   m_nextID{ 1 };
	std::unordered_map<std::string, SoundID> m_pathToID;
};

class DebugSoundSystem final : public SoundSystem
{
public:
	DebugSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		: m_pActualSoundSystem(std::move(soundSystem))
	{
	}
	DebugSoundSystem(const DebugSoundSystem&) = delete;
	DebugSoundSystem(DebugSoundSystem&&) = delete;
	DebugSoundSystem& operator=(const DebugSoundSystem&) = delete;
	DebugSoundSystem& operator=(DebugSoundSystem&&) = delete;
	~DebugSoundSystem() override = default;

	SoundID SoundToId(const std::string& filePath) override
	{
		std::cout << "Loading sound: " << filePath << std::endl;
		return m_pActualSoundSystem->SoundToId(filePath);
	}
	void PlaySound(SoundID soundId, int volume) override
	{
		std::cout << "Playing sound with ID: " << soundId << " at volume: " << volume << std::endl;
		m_pActualSoundSystem->PlaySound(soundId, volume);
	}
private :
	std::unique_ptr<SoundSystem> m_pActualSoundSystem;
};
