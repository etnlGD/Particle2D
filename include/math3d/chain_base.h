#ifndef MATH3D_CHAIN_BASE_H
#define MATH3D_CHAIN_BASE_H
#include <boost/operators.hpp>

namespace math3d 
{
	template <typename Derived, typename B = boost::detail::empty_base<Derived>>
	class chain_base : B
	{
	protected:
		typedef chain_base<Derived, B> ChainBase;
		typedef Derived MyType;

	protected:
		inline Derived* self() 
		{
			return static_cast<Derived*>(this);
		}

		inline const Derived* self() const 
		{
			return static_cast<const Derived*>(this);
		}

		inline Derived& ref() 
		{
			return static_cast<Derived&>(*this);
		}

		inline const Derived& ref() const 
		{
			return static_cast<const Derived&>(*this);
		}
	};
}

#endif
