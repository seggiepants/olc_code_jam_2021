#include <cmath>
#include "Collision.h"

namespace jam
{
	bool Collision::Circle_Circle(Point2Df* centerA, float radiusA, Point2Df* centerB, float radiusB)
	{
		float dx = abs(centerA->x - centerB->x);
		float dy = abs(centerA->y - centerB->y);
		float distA = (dx * dx) + (dy * dy);
		float distB = (radiusA * radiusA) + (radiusB * radiusB);

		return distA <= distB;
	}

	bool Collision::Circle_Point(Point2Df* center, float radius, Point2Df* point)
	{
		float dx = abs(center->x - point->x);
		float dy = abs(center->y - point->y);
		float distA = (dx * dx) + (dy * dy);
		float distB = (radius * radius);
		return distA <= distB;

	}

	bool Collision::Line_Line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	{
		float s02_x, s02_y, s10_x, s10_y;
		float s32_x, s32_y, s_numer, t_numer;
		float denom, t;
		bool denom_positive;

		// Bounding Box check.
		jam::Rect a, b;
		a.x = x1 <= x2 ? x1 : x2; a.w = std::abs(x2 - x1); a.y = y1 <= y2 ? y1 : y2; a.h = std::abs(y2 - y1);
		b.x = x3 <= x4 ? x3 : x4; b.w = std::abs(x4 - x3); b.y = y3 <= y4 ? y3 : y4; b.h = std::abs(y4 - y3);
		if (Collision::Rect_Rect(&a, &b))
		{
			s10_x = x2 - x1;
			s10_y = y2 - y1;
			s32_x = x4 - x3;
			s32_y = y4 - y3;

			denom = (s10_x * s32_y) - (s32_x * s10_y);
			if (denom == 0.0)
			{
				return false; // co-linear
			}
			denom_positive = denom > 0;
			s02_x = x1 - x3;
			s02_y = y1 - y3;
			s_numer = (s10_x * s02_y) - (s10_y * s02_x);
			if ((s_numer < 0.0) == denom_positive)
			{
				return false; // No collision
			}

			t_numer = (s32_x * s02_y) - (s32_y * s02_x);
			if ((t_numer < 0.0) == denom_positive)
			{
				return false; // No collision
			}

			if ((s_numer > denom) == denom_positive)
			{
				return false; // No collision
			}
		}
		else
		{
			return false;
		}

		// Collision detected
		return true;
	}

	bool Collision::Point_Rect(Point2Df* point, Rect* rect)
	{
		float x1, x2, y1, y2;
		x1 = rect->x;
		x2 = rect->x + rect->w;
		y1 = rect->y;
		y2 = rect->y + rect->h;
		return (point->x >= x1 && point->x <= x2 && point->y >= y1 && point->y <= y2);
	}

	bool Collision::Rect_Rect(Rect* a, Rect* b)
	{
		float x1, x2, y1, y2;
		float x3, x4, y3, y4;
		x1 = a->x;
		x2 = a->x + a->w;
		y1 = a->y;
		y2 = a->y + a->h;
		
		x3 = b->x;
		x4 = b->x + b->w;
		y3 = b->y;
		y4 = b->y + b->h;

		if (x2 < x3)
		{
			// Box A to right of Box B
			return false;
		}
		if (x1 > x4)
		{
			// Box A to the left of Box b
			return false;
		}

		if (y2 < y3)
		{
			// Box A above Box B
			return false;
		}
		if (y1 > y4)
		{
			// Box A below Box b
			return false;
		}
		return true;
	}

}