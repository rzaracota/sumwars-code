// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// GetTime.cpp
// File created January 24, 2003
// Simple class to get a high resolution system time
// ----------------------------------------------------------------------

#ifndef __GET_TIME_H
#define __GET_TIME_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

// A utility class to get a more accurate time than timeGetTime()
class GetTime
{
public:
	// Call this before using the function.  It won't work in constructor calls though, so you have to call this in Winmain or main and
	// not use the class in your constructors
	void Init(void);

	// Call this or use the #define getTime (below) to get the current time
	inline unsigned long Time(void)
	{
		if (initialized==false) Init();

		#ifdef _WIN32
		LARGE_INTEGER PerfVal;
		QueryPerformanceCounter(&PerfVal);
		return (unsigned long)(PerfVal.QuadPart/counts);
		#else
		gettimeofday(&tp, 0);
		// Seconds to ms and microseconds to ms
		return (tp.tv_sec - initialTime.tv_sec) * 1000 + (tp.tv_usec - initialTime.tv_usec) / 1000;
		#endif
	}

	GetTime();
	~GetTime() {}
	// static function because only static functions can access static members
	static inline GetTime* Instance() {return &I;}
private:
    static GetTime I;
    
    	#ifdef _WIN32
	LARGE_INTEGER yo;
	LONGLONG counts;
	#else
	timeval tp, initialTime;
	#endif
	bool initialized;
};

#define getTime GetTime::Instance()->Time

#endif

