#ifndef MATH3D_VECTOR4D_H
#define MATH3D_VECTOR4D_H
#include <cstdint>
#include "vector3d.h"

namespace math3d 
{
	template <class T> 
	class vector4d : public basic_vector<T, vector4d>
	{
#ifdef __GNUC__
		typedef typename basic_vector<T, vector4d>::BasicVector BasicVector;
#endif
		typedef vector4d<T> MyType;

	public:
		static const MyType ZERO;
		static const MyType UNIT_X;
		static const MyType UNIT_Y;
		static const MyType UNIT_Z;
		static const MyType UNIT_W;
		static const MyType NEGATIVE_UNIT_X;
		static const MyType NEGATIVE_UNIT_Y;
		static const MyType NEGATIVE_UNIT_Z;
		static const MyType NEGATIVE_UNIT_W;
		static const MyType UNIT_SCALE;
		enum { DIMENSION = 4 };

	public:
		inline vector4d(T value = 0) 
			: x(value), y(value), z(value), w(value)
		{
		}

		inline vector4d(T x, T y, T z, T w) 
			: x(x), y(y), z(z), w(w)
		{
		}

		inline explicit vector4d(const T data[4]) 
			: x(data[0]), y(data[1]), z(data[2]), w(data[3])
		{
		}

		template <typename U>
		inline vector4d(const vector4d<U>& rhs) 
			: x((T) rhs.x), y((T) rhs.y), z((T) rhs.z), w((T) rhs.w) 
		{
		}

		inline explicit vector4d(const vector3d<T>& rhs, T w = 1) 
			: x(rhs.x), y(rhs.y), z(rhs.z), w(w)
		{
		}

		inline vector3d<T> reduceD() const
		{
			vector3d<T> res(x, y, z);
			return res *= ((T) 1 / w);
		}

		template <typename U>
		inline MyType& operator=(const vector4d<U>& rhs) 
		{
			return set((T) rhs.x, (T) rhs.y, (T) rhs.z, (T) rhs.w);
		}

		inline MyType& set(T value) 
		{
			return set(value, value, value, value);
		}

		inline MyType& set(T x, T y, T z, T w) 
		{
			this->x = x; 
			this->y = y;
			this->z = z;
			this->w = w;
			return *this;
		}

		inline MyType& operator+=(const MyType& rhs) 
		{
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;
		}

		inline MyType& operator-=(const MyType& rhs) 
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;
		}

		inline MyType& operator*=(const MyType& rhs) 
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
			return *this;
		}

		using BasicVector::operator/=;
		inline MyType& operator/=(const MyType& rhs) 
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
			return *this;
		}

		inline bool operator==(const MyType& rhs) const 
		{
			return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
		}

		inline T dot(const MyType& rhs) const 
		{
			return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
		}

		inline MyType& negate() 
		{
			x = -x; y = -y; z = -z; w = -w;
			return *this;
		}

		inline T len2() const 
		{
			return x*x + y*y + z*z + w*w;
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

		union {
			T z;
			T p;
		};

		union {
			T w;
			T q;
		};
	};

	typedef vector4d<float>  vec4;
	typedef vector4d<double> dvec4;
	typedef vector4d<int32_t>ivec4;

	template <class T> 
	const vector4d<T> vector4d<T>::ZERO;

	template <class T> 
	const vector4d<T> vector4d<T>::UNIT_X(1, 0, 0, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::UNIT_Y(0, 1, 0, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::UNIT_Z(0, 0, 1, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::UNIT_W(0, 0, 0, 1);

	template <class T> 
	const vector4d<T> vector4d<T>::NEGATIVE_UNIT_X(-1, 0, 0, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::NEGATIVE_UNIT_Y(0, -1, 0, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::NEGATIVE_UNIT_Z(0, 0, -1, 0);

	template <class T> 
	const vector4d<T> vector4d<T>::NEGATIVE_UNIT_W(0, 0, 0, -1);

	template <class T> 
	const vector4d<T> vector4d<T>::UNIT_SCALE(1);

}

#endif


