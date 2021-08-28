#pragma once
#ifndef __IRENDERER_H__
#define __IRENDERER_H__
#include <vector>
#include "IImage.h"
#include "Point2D.h"
#include "RGB.h"

namespace jam
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() {};
		virtual void Clear(jam::rgb bg) = 0;
		virtual void DrawImage(jam::IImage* img, int x, int y) = 0;
		virtual void DrawLine(int x1, int y1, int x2, int y2, jam::rgb color) = 0;
		virtual void DrawPoint(int x, int y, jam::rgb color) = 0;
		virtual void DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color) = 0;
		virtual void DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h) = 0;
		virtual void FillRect(int x1, int y1, int x2, int y2, jam::rgb color) = 0;
		virtual void GetScreenSize(int* width, int* height) = 0;
	};
}

#endif