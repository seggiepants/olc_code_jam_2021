#pragma once
#ifndef __SHARED_H__
#define __SHARED_H__

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
}
#endif