#pragma once
#ifndef __IFONT_H__
#define __IFONT_H__

#include <string>
#include "IRenderer.h"

namespace jam
{
	class IFont
	{
	public:
		virtual ~IFont() {};
		virtual void DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color) = 0;
		virtual void MeasureText(std::string message, int* width, int* height) = 0;
	};
}
#endif
