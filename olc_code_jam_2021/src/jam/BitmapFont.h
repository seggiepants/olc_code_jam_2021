#pragma once
#ifndef __BITMAP_FONT_H__
#define __BITMAP_FONT_H__
#include <string>
#include <unordered_map>
#include "IFont.h"
#include "IImage.h"
#include "Rect.h"

namespace jam
{
    // Windows Header file was overshadowing my function
#ifdef DrawText
#undef DrawText
#endif
    class BitmapFont :
        public IFont
    {
    public:
        BitmapFont();
        ~BitmapFont();
        void Construct(std::string filePath);
        void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color);
        void MeasureText(std::string message, int* width, int* height);
    private:
        IImage* texture;
        std::unordered_map<char, jam::Rect> runes;
    };
}
#endif