#include <iostream>
#include "RendererPGE.h"
#include "../../Shared.h"
#include "TTFFontPGE.h"

namespace jam
{
	
	TTFFontPGE::TTFFontPGE()
	{
		this->ttfFont = nullptr;
	}

	TTFFontPGE::~TTFFontPGE()
	{
		if (this->ttfFont != nullptr)
		{
			delete this->ttfFont;
			this->ttfFont = nullptr;
		}
	}

	bool TTFFontPGE::Construct(std::string fileName, int pointSize)
	{
		this->ttfFont = new olc::Font(fileName, pointSize);
		if (this->ttfFont == nullptr)
		{
			std::cerr << "Failed to load font \"" << fileName << "\" Size " << pointSize << "." << std::endl;
			return false;
		}
		return true;
	}

	void TTFFontPGE::DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color)
	{
		int width, height;
		this->MeasureText(message, &width, &height);
		olc::Pixel fg = { color.r, color.g, color.b };
		std::u32string strBuffer(message.begin(), message.end());
		olc::FontRect size = this->ttfFont->GetStringBounds(strBuffer, 0.0f);
		olc::vi2d pos = { x, y - height };
		olc::Sprite* sprite = this->ttfFont->RenderStringToSprite(strBuffer, fg);
		((RendererPGE*)render)->GetApp()->DrawSprite(pos, sprite);
		delete sprite;
	}

	void TTFFontPGE::MeasureText(std::string message, int* width, int* height)
	{
		std::u32string strBuffer(message.begin(), message.end());
		olc::FontRect r = this->ttfFont->GetStringBounds(strBuffer);
		*width = r.size.x;
		*height = r.size.y;
	}
}