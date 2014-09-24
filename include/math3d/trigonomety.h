#ifndef MATH3D_TRIGONOMETRY_H
#define MATH3D_TRIGONOMETRY_H
#include "angle.h"
#include <cmath>

#define TRIGONOMETRY_FUNCTION_DEF(func)									\
	template <typename T> T func(const ::math3d::radian_t<T>& rad) {	\
		static_assert(std::is_floating_point<T>::value,					\
			"floating point radian only");								\
		return ::std::func(rad.value());								\
	}

#define TRIGONOMETRY_INV_FUNCTION_DEF(func)								\
	template <typename T> ::math3d::radian_t<T> func(T tval) {			\
		static_assert(std::is_floating_point<T>::value,					\
			"floating point radian only");								\
		return ::math3d::radian_t<T>(::std::func(tval));				\
	}

#define TRIGONOMETRY_INV_FUNCTION_2_DEF(func)							\
	template <typename T> ::math3d::radian_t<T> func(T tval1, T tval2) {\
		static_assert(std::is_floating_point<T>::value,					\
			"floating point radian only");								\
		return ::math3d::radian_t<T>(::std::func(tval1, tval2));		\
	}

namespace math3d {

	// Trigonometric functions:
	TRIGONOMETRY_FUNCTION_DEF(cos);
	TRIGONOMETRY_FUNCTION_DEF(sin);
	TRIGONOMETRY_FUNCTION_DEF(tan);
	TRIGONOMETRY_INV_FUNCTION_DEF(acos);
	TRIGONOMETRY_INV_FUNCTION_DEF(asin);
	TRIGONOMETRY_INV_FUNCTION_DEF(atan);

	TRIGONOMETRY_INV_FUNCTION_2_DEF(atan2);

	// Hyperbolic functions:
	TRIGONOMETRY_FUNCTION_DEF(cosh);
	TRIGONOMETRY_FUNCTION_DEF(sinh);
	TRIGONOMETRY_FUNCTION_DEF(tanh);
	TRIGONOMETRY_INV_FUNCTION_DEF(acosh);
	TRIGONOMETRY_INV_FUNCTION_DEF(asinh);
	TRIGONOMETRY_INV_FUNCTION_DEF(atanh);

}

#endif


