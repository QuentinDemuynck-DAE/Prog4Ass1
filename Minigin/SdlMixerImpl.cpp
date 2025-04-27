#include "DerivedSoundSystems.h"
#include "SDL_mixer.h"
#include <stdexcept>
#include <unordered_map>

class NoisySoundSystem::Impl
{
public:
	Impl()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			throw std::runtime_error("SDL_mixer could not initialize! SDL_mixer Error: " + std::string(Mix_GetError()));
		}
	}

	~Impl() 
	{
		Mix_CloseAudio();
	}

	Mix_Chunk* Load(const std::string& path) 
	{
		Mix_Chunk* c = Mix_LoadWAV(path.c_str());
		if (!c) throw std::runtime_error("Failed to load “" + path + "”: " + Mix_GetError());
		return c;
	}

	void Play(Mix_Chunk* c, int vol) 
	{
		Mix_VolumeChunk(c, vol);
		Mix_PlayChannel(-1, c, 0);
	}

	// store under your sound-ID
	void Store(unsigned int id, Mix_Chunk* chunk) 
	{
		m_chunks[id] = chunk;
	}

	// retrieve (or nullptr if not found)
	Mix_Chunk* Get(unsigned int id) const 
	{
		auto it = m_chunks.find(id);
		return it != m_chunks.end() ? it->second : nullptr;
	}

private:
	std::unordered_map<unsigned int, Mix_Chunk*> m_chunks;
};