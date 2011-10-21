/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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