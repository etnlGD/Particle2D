#ifndef MATH3D_MATH3D_H
#define MATH3D_MATH3D_H
#include <cassert>
#include <boost/call_traits.hpp>

namespace math3d 
{
	template <typename T>
	inline T clamp(
		typename boost::call_traits<T>::param_type pValue, 
		typename boost::call_traits<T>::param_type pMin, 
		typename boost::call_traits<T>::param_type pMax)
	{
		assert(pMax >= pMin);

		if (pValue > pMax)
			return pMax;
		else if (pValue < pMin)
			return pMin;
		else
			return pValue;
	}

}

#endif
