#ifndef MATH3D_BASIC_VECTOR_H
#define MATH3D_BASIC_VECTOR_H
#include <cmath>
#include "angle.h"
#include "math3d.h"
#include "chain_base.h"

namespace math3d 
{
	template <typename T, template <class> class Vector>
	class basic_vector : 
		public chain_base<Vector<T>,
			boost::arithmetic<Vector<T>,
				boost::equality_comparable<Vector<T> > > >
	{
	protected:	// typedefs
		typedef Vector<T> MyType;
		typedef basic_vector<T, Vector> BasicVector;

	private:	// traits
		/**
		 * Nested template class specilization is not allowed, so we must 
		 * add a `_dummy` parameter to simulate partial specilization.
		 */
		template <bool isFloatingPoint, typename _dummy = void> 
		struct div_value_traits
		{
		};

		template <typename _dummy> 
		struct div_value_traits<true, _dummy>
		{
		public:
			static MyType& div(MyType& vec, T value)
			{
				T inv = (T) 1 / value;
				return vec *= inv;
			}
		};

		template <typename _dummy> 
		struct div_value_traits<false, _dummy>
		{
		public:
			static MyType& div(MyType& vec, T value)
			{
				return vec /= MyType(value);
			}
		};

		enum { isFloatingPoint = std::is_floating_point<T>::value };

		typedef div_value_traits<isFloatingPoint> DivTratis;

		inline T _fast_test_len() const
		{
			T length = 0;
			const T *pdata = ptr();
			for (int i = 0; i < MyType::DIMENSION; ++i)
			{
				T d = pdata[i];
				if (d != 0)
				{
					if (length == 0)
						length = d;
					else // Failed
						return -1;
				}
			}

			return ::std::abs(length);
		}

	protected:
		// Requires T is floating point type
		inline void _check_T_is_floating_point() const
		{
			static_assert(isFloatingPoint, 
				"This method is not safe to integral vector, you may convert"
				" it to a floating point vector first");
		}

	public:
		inline basic_vector() 
		{
			static_assert(is_nonbool_arithmetic<T>::value, 
				"T must be arithmetic value");
		}

		/**
		 * Constants to implement:
		 * DIMENSION
		 * 
		 * Functions to implement:
		 * Vector(const Vector<U>&)
		 * operator=(const Vector<U>&)
		 * operator==(const MyType&)
		 * operator-=(const MyType&)
		 * operator+=(const MyType&)
		 * operator*=(const MyType&)
		 * operator/=(const MyType&)
		 * len2()
		 * dot(const MyType&)
		 */

		inline T operator[](int idx) const 
		{
			assert(idx >= 0 && idx < MyType::DIMENSION);
			return ptr()[idx];
		}

		/// Optimize for div value
		/**
		 * Note that operator/(value) is not optimized, it will invoke 
		 * operator/(const MyTyp&) instead.
		 */
		inline MyType& operator/=(T value) 
		{
			return DivTratis::div(this->ref(), value);
		}

		inline T* ptr()
		{
			return reinterpret_cast<T*>(this);
		}

		inline const T* ptr() const
		{
			return reinterpret_cast<const T*>(this);
		}

		inline T dst2(const MyType& rhs) const 
		{
			return (rhs - this->ref()).len2();
		}

		/// Return if the vector is perpendicular to rhs
		inline bool perp(const MyType& rhs) const
		{
			return this->self()->dot(rhs) == 0;
		}

		inline bool isZero() const
		{
			return this->ref() == MyType::ZERO;
		}

		inline MyType operator-() const 
		{
			MyType res(this->ref());
			return res.negate();
		}

		inline T dst(const MyType& rhs) const 
		{
			_check_T_is_floating_point();
			return (T) ::std::sqrt(dst2(rhs));
		}

		T len() const 
		{
			_check_T_is_floating_point();
			T length = _fast_test_len();
			if (length < 0)	// we must do sqrt
				length = ::std::sqrt(this->self()->len2());
			
			return length;
		}

		inline MyType& setLen(T length) 
		{
			_check_T_is_floating_point();
			if (!this->ref().isZero())
				return this->ref() *= (length / len());
			else
				return this->ref();
		}

		// This method has moved to "math3d.h"
		// inline MyType lerp(const MyType& vec, T ratio) const 
		// {
		// 	_check_T_is_floating_point();
		// 	MyType res = this->ref() * ratio;
		// 	return res += (vec * (1 - ratio));
		// }

		inline MyType mid(const MyType& vec) const 
		{
			_check_T_is_floating_point();
			return this->self()->lerp(vec, (T) 0.5f);
		}

		inline MyType dir() const 
		{
			_check_T_is_floating_point();
			return MyType(this->ref()).nor();
		}

		inline MyType& nor() 
		{
			_check_T_is_floating_point();
			if (!this->self()->isZero()) 
				return this->self()->operator/=(len());

			return this->ref();
		}

		inline MyType reflectN(const MyType& normal) const
		{
			_check_T_is_floating_point();
			MyType reflected = normal * (-2 * this->self()->dot(normal));
			return reflected += this->ref();
		}

		/**
		 * Refract this on surface with normal.
		 * 
		 * @note this and normal must be normalized
		 * @param eta (sinOut / sinIn)
		 * @return The normalized refracted direction
		 */
		inline MyType refractN(const MyType& normal, const T eta) const
		{
			_check_T_is_floating_point();
			const T eta_x_cos1 = eta * this->self().dot(normal);

			// i.e.: 
			//	1.0 - eta^2 * (1.0 - cos1^2);
			//	1.0 - (eta*sin1)^2
			//	1.0 - (sin2)^2
			const T cos2Square =(T) 1 - eta*eta + eta_x_cos1*eta_x_cos1;
			if (cos2Square < (T) 0) 
				return MyType((T) 0);

			T cos2 = ::std::sqrt(cos2Square);

			// x-component is: eta * this - eta * (cosIn * normal);
			// y-component is: cosOut * normal;
			MyType result = (*this->ref()) * eta;
			return (result -= normal * (eta_x_cos1 + cos2));
		}

		inline radian_t<T> angleBetween(const MyType& vec) const
		{
			_check_T_is_floating_point();
			MyType n = dir();
			return n.angleBetweenN(vec.dir());
		}

		inline radian_t<T> angleBetweenN(const MyType& normalized) const
		{
			_check_T_is_floating_point();
			T cos = clamp<T>(this->self()->dot(normalized), -1, 1);
			return radian_t<T>((T) ::std::acos(cos));
		}
	};

}

#endif
