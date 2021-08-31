#pragma once
#ifndef __SHARED_H__
#define __SHARED_H__

#include <string>
#include "backend/IBackend.h"

#if defined(__UNIX__)
	#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define OS_WIN
#else
	#define OS_UNKNOWN
#endif

namespace jam
{
	extern IBackend* backEnd;
	const std::string CONFIG_PATH = "assets/config/";
	const std::string FONT_PATH = "assets/font/";
	const std::string IMAGE_PATH = "assets/image/";
	const std::string MAP_PATH = "assets/map/";
	const std::string SOUND_PATH = "assets/sound/";
}
#endif