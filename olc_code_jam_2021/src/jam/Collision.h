#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Point2D.h"
#include "Rect.h"

namespace jam
{
	class Collision
	{
	public:
		static bool Circle_Circle(Point2Df* centerA, float radiusA, Point2Df* centerB, float radiusB);
		static bool Circle_Point(Point2Df* center, float radius, Point2Df* point);
		static bool Line_Line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
		static bool Point_Rect(Point2Df* point, Rect* rect);
		static bool Rect_Rect(Rect* a, Rect* b);
	};
}
#endif