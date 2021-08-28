#pragma once

#ifndef __AUDIO_PGE_H__
#define __AUDIO_PGE_H__

#include <string>
#include "../../IAudio.h"

namespace jam
{
    class AudioPGE :
        public IAudio
    {
    public:
        AudioPGE();
        ~AudioPGE();
        bool Load(std::string fileName);
        void Play();
        void Stop();
    protected:
        int handle;
    };
}
#endif