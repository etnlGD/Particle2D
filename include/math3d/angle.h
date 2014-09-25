#ifndef MATH3D_ANGLE_H
#define MATH3D_ANGLE_H
#include <cmath>
#include <cassert>
#include "nonbool_traits.h"
#include "oneway_operators.h"

namespace math3d 
{
	// Forward decleration of degree_t
	template <typename T> class degree_t;

	template <typename T>
	class radian_t :
		public boost::multiplicative2<radian_t<T>, T, 
			oneway_additive<radian_t<T>, 
				oneway_totally_ordered<radian_t<T> > > >
	{
		typedef radian_t<T> MyType;
		typedef MyType MyConstants;

	public:
		static const MyType ZERO;
		static const MyType QUATER_PI;
		static const MyType HALF_PI;
		static const MyType PI;
		static const MyType TWO_PI;

		static const T TO_DEGREE;

	public:
		explicit inline radian_t(T radValue = 0) : mValue(radValue) 
		{
			static_assert(is_nonbool_arithmetic<T>::value,
				"T must be arithmetic value");
		}

		inline radian_t(const degree_t<T>& d);

		inline T value() const 
		{
			return mValue;
		}

		inline bool normalized() const 
		{
			return mValue > -PI.value() 
				&& mValue <= PI.value();
		}

		void nor() 
		{
			if (!normalized())
			{
				mValue = fmod(mValue, PI.value());
				if (mValue < -PI.value())
					mValue = mValue + TWO_PI.value();
				else if(mValue >= PI.value())
					mValue = mValue - PI.value();

				assert(normalized());
			}
		}

		inline bool operator<(const MyType& rhs) const 
		{
			return mValue < rhs.mValue;
		}

		inline bool operator==(const MyType& rhs) const
		{
			return mValue == rhs.mValue;
		}

		inline MyType& operator+=(const MyType& rhs) 
		{
			mValue += rhs.mValue;
			return *this;
		}

		inline MyType& operator-=(const MyType& rhs) 
		{
			mValue -= rhs.mValue;
			return *this;
		}

		inline MyType& operator*=(T rhs)
		{
			mValue *= rhs;
			return *this;
		}

		inline MyType& operator/=(T rhs)
		{
			mValue /= rhs;
			return *this;
		}

		inline MyType operator-() const
		{
			return MyType(-mValue);
		}

	private:
		T mValue;
	};

	template <typename T>
	class degree_t :
			public oneway_additive<degree_t<T>,
				oneway_totally_ordered<degree_t<T> > >
	{
		typedef degree_t<T> MyType;

	public:
		static const MyType ZERO;
		static const MyType DEGREE_45;
		static const MyType DEGREE_90;
		static const MyType DEGREE_180;
		static const MyType DEGREE_360;

		static const T TO_RADIAN;

	public:
		explicit inline degree_t(T degValue = 0) : mValue(degValue) 
		{
			static_assert(std::is_floating_point<T>::value,
				"T must be arithmetic value");
		}

		inline degree_t(const radian_t<T>& r) 
			: mValue(r.value() * radian_t<T>::TO_DEGREE)
		{
		}

		inline T value() const
		{ 
			return mValue; 
		}

		inline bool normalized() const 
		{
			return mValue > -180 
				&& mValue <= 180;
		}

		void nor() 
		{
			if (!normalized())
			{
				mValue = fmod(mValue, (T) 360);
				if (mValue < -180)
					mValue = mValue + 360;
				else if(mValue >= 180)
					mValue = mValue - 360;

				assert(normalized());
			}
		}

		inline bool operator<(const MyType& rhs) const 
		{
			return mValue < rhs.mValue;
		}

		inline bool operator==(const MyType& rhs) const
		{
			return mValue == rhs.mValue;
		}

		inline MyType& operator+=(const MyType& rhs) 
		{
			mValue += rhs.mValue;
			return *this;
		}

		inline MyType& operator-=(const MyType& rhs) 
		{
			mValue -= rhs.mValue;
			return *this;
		}

		inline MyType& operator*=(T rhs)
		{
			mValue *= rhs;
			return *this;
		}

		inline MyType& operator/=(T rhs)
		{
			mValue /= rhs;
			return *this;
		}

		/// Scale degree
		/**
		 * Only support `degree * float`. 
		 * 
		 * @return scaled degree
		 */
		inline MyType operator*(T rhs) const
		{
			return MyType(mValue * rhs);
		}

		/// Scale degree
		/**
		 * @see #operator*(T)
		 */
		inline MyType operator/(T rhs) const 
		{
			return MyType(mValue / rhs);
		}

		inline MyType operator-() const
		{
			return MyType(-mValue);
		}

	private:
		T mValue;
	};

	template <typename T>
	radian_t<T>::radian_t(const degree_t<T>& d) 
		: mValue(d.value() * degree_t<T>::TO_RADIAN) 
	{
	}

	typedef degree_t<float> degree;
	typedef radian_t<float> radian;

	typedef degree_t<double> degree64;
	typedef radian_t<double> radian64;


#ifndef MATH3D_PI_VALUE
#define MATH3D_PI_VALUE 3.1415926535897932384626433832795028841971693993751058
#endif
	template <typename T>
	const radian_t<T> radian_t<T>::ZERO(0);

	template <typename T>
	const radian_t<T> radian_t<T>::PI(T(MATH3D_PI_VALUE));

	template <typename T>
	const radian_t<T> radian_t<T>::QUATER_PI(T(0.25 * MATH3D_PI_VALUE));

	template <typename T>
	const radian_t<T> radian_t<T>::HALF_PI(T(0.5 * MATH3D_PI_VALUE));

	template <typename T>
	const radian_t<T> radian_t<T>::TWO_PI(T(2 * MATH3D_PI_VALUE));

	template <typename T>
	const T radian_t<T>::TO_DEGREE = T(180 / MATH3D_PI_VALUE);



	template <typename T>
	const degree_t<T> degree_t<T>::ZERO(0);

	template <typename T>
	const degree_t<T> degree_t<T>::DEGREE_45(45);

	template <typename T>
	const degree_t<T> degree_t<T>::DEGREE_90(90);

	template <typename T>
	const degree_t<T> degree_t<T>::DEGREE_180(180);

	template <typename T>
	const degree_t<T> degree_t<T>::DEGREE_360(360);

	template <typename T>
	const T degree_t<T>::TO_RADIAN = T(MATH3D_PI_VALUE / 180);
}

#endif


