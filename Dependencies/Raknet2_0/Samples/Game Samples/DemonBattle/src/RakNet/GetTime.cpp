// ----------------------------------------------------------------------
// This API and the code herein created by and wholly and privately owned by Kevin Jenkins except where specifically indicated otherwise.
// Licensed under the "RakNet" brand by Rakkarsoft LLC and subject to the terms of the relevant licensing agreement available at http://www.rakkarsoft.com
// GetTime.cpp
// File created January 24, 2003
// Simple class to get a high resolution system time
// ----------------------------------------------------------------------

#include "GetTime.h"

////#include "MemoryManager.h"

GetTime GetTime::I;

GetTime::GetTime()
{
// For some reason this fails in the constructor
//	QueryPerformanceFrequency(&yo);
//	counts = yo.QuadPart >> 10;
	initialized=false;
}

void GetTime::Init(void)
{
	#ifdef _WIN32
	QueryPerformanceFrequency(&yo);
	// The original code shifted right 10 bits
	//counts = yo.QuadPart >> 10;
	// It gives the wrong value since 2^10 is not 1000
	counts = yo.QuadPart / 1000;
	#else
	gettimeofday(&initialTime, 0); 
	#endif
	initialized=true; 
}
