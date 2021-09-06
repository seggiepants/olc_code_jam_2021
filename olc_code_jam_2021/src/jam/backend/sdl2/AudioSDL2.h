#pragma once
#ifndef __AUDIOSDL2_H__
#define __AUDIOSDL2_H__
#include <string>
#include <SDL2/SDL_mixer.h>
#include "../../IAudio.h"

namespace jam
{
    class AudioSDL2 :
        public IAudio
    {
    public:
        AudioSDL2();
        ~AudioSDL2();
        bool Load(std::string fileName);
        void Play();
        void Stop();
    protected:
        Mix_Chunk* chunk;
        int channel;
    };
}
#endif

