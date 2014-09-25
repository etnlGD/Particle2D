#ifndef MATH3D_MATH3D_H
#define MATH3D_MATH3D_H
#include <cassert>

namespace math3d 
{
	template <typename T>
	inline T clamp(const T& value, const T& min, const T& max)
	{
		assert(max >= min);

		if (value > max)
			return max;
		else if (value < min)
			return min;
		else
			return value;
	}
	
	template <typename T>
	inline void doClamp(T& value, const T& min, const T& max)
	{
		assert(max >= min);

		if (value > max)
			value = max;
		else if (value < min)
			value = min;
	}

#pragma warning(disable: 4244)
	template <typename T>
	inline T lerp(const T& source, const T& target, float progress)
	{
		T result = target - source;
		result *= progress;
		result += source;
		return result;
	}

}

#endif
