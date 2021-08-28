#include "RGB.h"

namespace jam
{
	rgb::rgb()
	{
		rgb(255, 255, 255, 255);
	}

	rgb::rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
}