#pragma once
#ifndef __RGB_H__
#define __RGB_H__
namespace jam
{
	struct rgb
	{
	public:
		rgb();
		rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
}
#endif
