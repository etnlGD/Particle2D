#ifndef MATH3D_VECTOR2D_H
#define MATH3D_VECTOR2D_H
#include <cstdint>
#include "basic_vector.h"

namespace math3d 
{
	template <class T> 
	class vector2d : public basic_vector<T, vector2d>
	{
#ifdef __GNUC__
		typedef typename basic_vector<T, vector2d>::BasicVector BasicVector;
#endif
		typedef vector2d<T> MyType;

	public:
		static const MyType ZERO;
		static const MyType UNIT_X;
		static const MyType UNIT_Y;
		static const MyType NEGATIVE_UNIT_X;
		static const MyType NEGATIVE_UNIT_Y;
		static const MyType UNIT_SCALE;
		enum { DIMENSION = 2 };

	public:
		inline vector2d(T value = 0) 
			: x(value), y(value) 
		{
		}

		inline vector2d(T x, T y) 
			: x(x), y(y) 
		{
		}

		inline explicit vector2d(const T data[2]) 
			: x(data[0]), y(data[1]) 
		{
		}

		template <typename U>
		inline vector2d(const vector2d<U>& rhs) 
			: x((T) rhs.x), y((T) rhs.y) 
		{
		}

		template <typename U>
		inline MyType& operator=(const vector2d<U>& rhs) 
		{
			return set((T) rhs.x, (T) rhs.y);
		}

		inline MyType& set(T value) 
		{
			return set(value, value);
		}

		inline MyType& set(T x, T y) 
		{
			this->x = x; 
			this->y = y;
			return *this;
		}

		inline MyType& operator+=(const MyType& rhs) 
		{
			x += rhs.x; y += rhs.y;
			return *this;
		}

		inline MyType& operator-=(const MyType& rhs) 
		{
			x -= rhs.x; y -= rhs.y;
			return *this;
		}

		inline MyType& operator*=(const MyType& rhs) 
		{
			x *= rhs.x; y *= rhs.y;
			return *this;
		}
		
		using BasicVector::operator/=;
		inline MyType& operator/=(const MyType& rhs) 
		{
			x /= rhs.x; y /= rhs.y;
			return *this;
		}

		inline bool operator==(const MyType& rhs) const 
		{
			return x == rhs.x && y == rhs.y;
		}

		inline T cross(const MyType& rhs) const 
		{
			return x*rhs.y - y*rhs.x;
		}

		inline T dot(const MyType& rhs) const 
		{
			return x*rhs.x + y*rhs.y;
		}

		inline MyType& negate() 
		{
			x = -x; y = -y;
			return *this;
		}

		inline T len2() const 
		{
			return x*x + y*y;
		}

		inline MyType perpendicular() const
		{
			return MyType(-y, x);
		}

	public:	// fields
		union {
			T x;
			T s;
		};

		union {
			T y;
			T t;
		};
	};

	typedef vector2d<float>  vec2;
	typedef vector2d<double> dvec2;
	typedef vector2d<int32_t>ivec2;

	template <class T> 
	const vector2d<T> vector2d<T>::ZERO;

	template <class T> 
	const vector2d<T> vector2d<T>::UNIT_X(1, 0);

	template <class T> 
	const vector2d<T> vector2d<T>::UNIT_Y(0, 1);

	template <class T> 
	const vector2d<T> vector2d<T>::NEGATIVE_UNIT_X(-1, 0);

	template <class T> 
	const vector2d<T> vector2d<T>::NEGATIVE_UNIT_Y(0, -1);

	template <class T> 
	const vector2d<T> vector2d<T>::UNIT_SCALE(1);

}

#endif
