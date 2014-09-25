#ifndef MATH3D_VECTOR3D_H
#define MATH3D_VECTOR3D_H
#include <cstdint>
#include "vector2d.h"

namespace math3d 
{
	template <class T> 
	class vector3d : public basic_vector<T, vector3d<T> >
	{
        typedef vector3d<T> MyType;
		typedef basic_vector<T, MyType> BasicVector;

	public:
		static const MyType ZERO;
		static const MyType UNIT_X;
		static const MyType UNIT_Y;
		static const MyType UNIT_Z;
		static const MyType NEGATIVE_UNIT_X;
		static const MyType NEGATIVE_UNIT_Y;
		static const MyType NEGATIVE_UNIT_Z;
		static const MyType UNIT_SCALE;
		enum { DIMENSION = 3 };

	public:
		inline vector3d(T value = 0) 
			: x(value), y(value), z(value) 
		{
		}

		inline vector3d(T x, T y, T z) 
			: x(x), y(y), z(z)
		{
		}

		inline explicit vector3d(const T data[3]) 
			: x(data[0]), y(data[1]), z(data[2]) 
		{
		}

		template <typename U>
		inline vector3d(const vector3d<U>& rhs) 
			: x((T) rhs.x), y((T) rhs.y), z((T) rhs.z) 
		{
		}

		inline explicit vector3d(const vector2d<T>& rhs, T z = 1) 
			: x(rhs.x), y(rhs.y), z(z)
		{
		}

		inline vector2d<T> reduceD() const
		{
			vector2d<T> res(x, y);
			return res *= ((T) 1 / z);
		}

		template <typename U>
		inline MyType& operator=(const vector3d<U>& rhs) 
		{
			return set((T) rhs.x, (T) rhs.y, (T) rhs.z);
		}

		inline MyType& set(T value) 
		{
			return set(value, value, value);
		}

		inline MyType& set(T x, T y, T z) 
		{
			this->x = x; 
			this->y = y;
			this->z = z;
			return *this;
		}

		inline MyType& operator+=(const MyType& rhs) 
		{
			x += rhs.x; y += rhs.y; z += rhs.z;
			return *this;
		}

		inline MyType& operator-=(const MyType& rhs) 
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z;
			return *this;
		}

		inline MyType& operator*=(const MyType& rhs) 
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z;
			return *this;
		}

		using BasicVector::operator/=;
		inline MyType& operator/=(const MyType& rhs) 
		{
			x /= rhs.x; y /= rhs.y; z /= rhs.z;
			return *this;
		}

		inline bool operator==(const MyType& rhs) const 
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}

		inline MyType cross(const MyType& rhs) const 
		{
			return MyType(y*rhs.z - z*rhs.y,
						  z*rhs.x - x*rhs.z,
						  x*rhs.y - y*rhs.x);
		}

		inline T dot(const MyType& rhs) const 
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		}

		inline MyType& negate() 
		{
			x = -x; y = -y; z = -z;
			return *this;
		}

		inline T len2() const 
		{
			return x*x + y*y + z*z;
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
	};

	typedef vector3d<float>  vec3;
	typedef vector3d<double> dvec3;
	typedef vector3d<int32_t>ivec3;

	template <class T> 
	const vector3d<T> vector3d<T>::ZERO;

	template <class T> 
	const vector3d<T> vector3d<T>::UNIT_X(1, 0, 0);

	template <class T> 
	const vector3d<T> vector3d<T>::UNIT_Y(0, 1, 0);

	template <class T> 
	const vector3d<T> vector3d<T>::UNIT_Z(0, 0, 1);

	template <class T> 
	const vector3d<T> vector3d<T>::NEGATIVE_UNIT_X(-1, 0, 0);

	template <class T> 
	const vector3d<T> vector3d<T>::NEGATIVE_UNIT_Y(0, -1, 0);

	template <class T> 
	const vector3d<T> vector3d<T>::NEGATIVE_UNIT_Z(0, 0, -1);

	template <class T> 
	const vector3d<T> vector3d<T>::UNIT_SCALE(1);

}

#endif
