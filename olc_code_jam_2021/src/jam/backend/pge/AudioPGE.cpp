#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include <olcPGEX_AudioConverter.h>
#include "AudioPGE.h"
#include "../../Shared.h"

namespace jam
{
	AudioPGE::AudioPGE() 
	{
		this->handle = -1;
	}

	AudioPGE::~AudioPGE()
	{
		this->Stop();
	}

	bool AudioPGE::Load(std::string fileName)
	{
		int sampleID = olc::AudioConvert::LoadAudioSample(fileName);
		if (sampleID < 0) {
			std::cout << "Failed to load sound \"" << fileName << "\"" << std::endl;
			return false;
		}
		else
		{
			this->handle = sampleID;
		}
		return true;
	}

	void AudioPGE::Play()
	{
		if (this->handle >= 0)
		{
			olc::SOUND::PlaySample(this->handle, false);
		}
	}

	void AudioPGE::Stop()
	{
		if (this->handle >= 0)
		{
			olc::SOUND::StopSample(this->handle);
		}
	}
}