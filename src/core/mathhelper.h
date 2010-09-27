#ifndef MATHHELPER_H
#define MATHHELPER_H

#ifdef WIN32
	#include <math.h>
	inline double roundf(double x) { return floor(x + 0.5); } // http://www.gamedev.net/community/forums/topic.asp?topic_id=436496
#endif

namespace MathHelper
{
	static inline double Max(double x, double y) { return x > y ? x : y; }
	static inline double Min(double x, double y) { return x < y ? x : y; }

//	static inline float Max(float x, float y) { return x > y ? x : y; }
//	static inline float Min(float x, float y) { return x < y ? x : y; }

	static inline int Max(int x, int y) { return x > y ? x : y; }
	static inline int Min(int x, int y) { return x < y ? x : y; }

	static inline unsigned int Max(unsigned int x, unsigned int y) { return x > y ? x : y; }
	static inline unsigned int Min(unsigned int x, unsigned int y) { return x < y ? x : y; }
}
#endif