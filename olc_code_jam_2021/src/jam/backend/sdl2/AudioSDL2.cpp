#include <iostream>
#include "AudioSDL2.h"

namespace jam
{
	AudioSDL2::AudioSDL2()
	{
		this->chunk = nullptr;
		this->channel = -1;
	}

	AudioSDL2::~AudioSDL2()
	{
		if (this->chunk != nullptr)
		{
			Mix_FreeChunk(this->chunk);
			this->chunk = nullptr;
		}
	}
	bool AudioSDL2::Load(std::string fileName)
	{
		bool ret = false;

		if (this->chunk != nullptr)
		{
			Mix_FreeChunk(this->chunk);
			this->chunk = nullptr;
		}

		this->chunk = Mix_LoadWAV(fileName.c_str());

		if (this->chunk == nullptr) 
		{
			std::cout << "Failed to load sound \"" << fileName << "\". Error: \"" << SDL_GetError() << "\"" << std::endl;
		}
		else
		{
			ret = true;
		}
		return ret;
	}

	void AudioSDL2::Play()
	{
		if (this->chunk != nullptr)
		{
			if (this->channel != -1)
			{
				if (Mix_Playing(this->channel))
				{
					Mix_HaltChannel(this->channel);
				}
			}
		}
		this->channel = Mix_PlayChannel(-1, this->chunk, 0);
		if (this->channel == -1)
		{
			std::cout << "Unable to play sound" << std::endl;
		}
	}

	void AudioSDL2::Stop()
	{
		if (this->chunk != nullptr && this->channel != -1)
		{
			Mix_HaltChannel(this->channel);
			this->channel = -1;
		}
	}
}