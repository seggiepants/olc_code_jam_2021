#pragma once
#ifndef __IIMAGE_H__
#define __IIMAGE_H__

namespace jam
{
	class IImage
	{
	public:
		virtual ~IImage() {};
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
	};
}
#endif

