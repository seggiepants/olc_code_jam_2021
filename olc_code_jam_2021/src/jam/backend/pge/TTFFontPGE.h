#pragma once
#ifndef __TTF_FONT_PGE_H__
#define __TTF_FONT_PGE_H__

#include <string>
#include <olcPGEX_TTF.h>
#include "../../IFont.h"
#include "../../IRenderer.h"
#include "../../RGB.h"

namespace jam
{
    class TTFFontPGE :
        public IFont
    {
    public:
        TTFFontPGE();
        ~TTFFontPGE();
        bool Construct(std::string fileName, int pointSize);
        void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color);
        void MeasureText(std::string message, int* width, int* height);
    protected:
        olc::Font* ttfFont;
    };
}
#endif
