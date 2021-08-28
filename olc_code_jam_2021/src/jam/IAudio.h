#pragma once
#ifndef __IAUDIO_H__
#define __IAUDIO_H__
#include <string>

namespace jam
{
	class IAudio
	{
	public:
		IAudio() {};
		virtual ~IAudio() {};
		virtual void Play() = 0;
		virtual void Stop() = 0;
	};
}

#endif