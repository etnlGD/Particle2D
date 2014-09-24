#ifndef MATH3D_NONBOOL_TRAITS_H
#define MATH3D_NONBOOL_TRAITS_H
#include <type_traits>

namespace math3d 
{
	template <typename T>
	struct is_nonbool_arithmetic : 
		public std::integral_constant<bool, std::is_arithmetic<T>::value>
	{
	};

	template <>
	struct is_nonbool_arithmetic<bool> :
		public std::integral_constant<bool, false>
	{
	};

}

#endif
