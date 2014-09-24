#ifndef MATH3D_ONEWAY_OPERATORS_H
#define MATH3D_ONEWAY_OPERATORS_H
#include <boost/operators.hpp>

namespace math3d 
{
	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_comparable : public B
	{
		inline const MyType& toMyType() const 
		{
			return static_cast<const MyType&>(*this);
		}

	public:
		inline bool operator>(const MyType& rhs) const 
		{
			return rhs < toMyType();
		}

		inline bool operator>=(const MyType& rhs) const 
		{
			return !static_cast<bool>(toMyType() < rhs);
		}

		inline bool operator<=(const MyType& rhs) const 
		{
			return !static_cast<bool>(rhs < toMyType());
		}
	};

	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_equivalent : public B
	{
	public:
		inline bool operator!=(const MyType& rhs) const 
		{
			return !static_cast<bool>(rhs == static_cast<const MyType&>(*this));
		}
	};

	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_totally_ordered :
		public oneway_comparable<MyType, oneway_equivalent<MyType, B> >
	{
	};

	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_addable : public B
	{
	public:
		inline MyType operator+(const MyType& rhs) const 
		{
			MyType sum(static_cast<const MyType&>(*this));
			sum += rhs;
			return sum;
		}
	};

	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_substractable : public B
	{
	public:
		MyType operator-(const MyType& rhs) const 
		{
			MyType res(static_cast<const MyType&>(*this));
			res -= rhs;
			return res;
		}
	};

	template <typename MyType, class B = ::boost::detail::empty_base<MyType>>
	class oneway_additive : 
		public oneway_addable<MyType, oneway_substractable<MyType, B>>
	{
	};

}

#endif
