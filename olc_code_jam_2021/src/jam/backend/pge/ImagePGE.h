#pragma once
#ifndef __IMAGE_PGE_H__
#define __IMAGE_PGE_H__

#include <string>
#include <olcPixelGameEngine.h>
#include "../../IRenderer.h"
#include "../../IImage.h"

namespace jam
{
    class ImagePGE :
        public IImage
    {
    public:
        ImagePGE();
        ~ImagePGE();
        bool Construct(std::string fileName);
        int GetWidth();
        olc::Sprite* GetHandle();
        int GetHeight();
    protected:
        olc::Sprite* LoadSprite(std::string fileName); 
        int width;
        int height;
        int bpp;
        olc::Sprite* sprite;
        //olc::Decal* decal;
    };
}

#endif