#ifndef MATH3D_QUATERNION_H
#define MATH3D_QUATERNION_H
#include "vector3d.h"

namespace math3d
{
	template <typename T>
	class quaternion
	{
	public:
		quaternion() {}

		quaternion(T x, T y, T z, T w) 
		{
			set(x, y, z, w);
		}

		quaternion(const vector3d<T>& axis, const radian_t<T>& angle) 
		{
			radian_t<T> half = angle * (T) 0.5;
			float s = ::math3d::sin(half);
			set(s * axis.x, s * axis.y, s * axis.z, ::math3d::cos(half));
		}

// 		explicit quaternion(const matrix& matrix)
// 		{
// 			float trace = matrix.trace();		
// 
// 			if (trace >= 1) 
// 			{
// 				float w4 = 4*w;
// 				x = (matrix(1, 2) - matrix(2, 1)) / w4;
// 				y = (matrix(2, 0) - matrix(0, 2)) / w4;
// 				z = (matrix(0, 1) - matrix(1, 2)) / w4;
// 				w = sqrt(trace) / 2;
// 			} 
// 			else
// 			{
// 				int max = matrix(0, 0) >= matrix(1, 1) ? 0 : 1;
// 				int mi1 = 1 - max;
// 
// 				max = matrix(max, max) >= matrix(2, 2) ? max : 2;
// 				int mi2 = 3 - mi1 - max;
// 
// 				float* ref = reinterpret_cast<float*> (this);
// 
// 				ref[max] = sqrt(matrix(max, max) - matrix(mi1, mi1) - matrix(mi2, mi2) + 1) / 2;
// 
// 				float num = 0.25f * ref[max];
// 				ref[mi1] = (matrix(max, mi1) + matrix(mi1, max)) * num;
// 				ref[mi2] = (matrix(max, mi2) + matrix(mi2, max)) * num;
// 				w		 = (matrix(mi1, mi2) - matrix(mi2, mi1)) * num;
// 			}
// 		}

		quaternion(const vector3d<T>& from, const vector3d<T>& to)
		{		
			vector3d<T> v0 = from.dir();
			vector3d<T> v1 = to.dir();

			const T d = v0.dot(v1);
			if (d >= 1.0f) // If dot == 1, vectors are the same
			{
				makeIdentity();
			}
			else if (d <= -1.0f) // exactly opposite
			{	
				// we won't use v1 anymore, so let it stand for the axis.
				vector3d<T>& axis = v1.set(1, 0, 0);
				axis = axis.cross(v0);
				if (axis.len() == 0)
				{
					axis.set(0.f,1.f,0.f);
					axis.cross(v0);
				}

				// same as fromAngleAxis(core::PI, axis).normalize();
				set(axis.x, axis.y, axis.z, 0).normalize();
			} 
			else 
			{
				const float s = ::std::sqrt((1+d) * 2);
				const float invs = 1.f / s;
				const vector3d<T> c = v0.cross(v1) * invs;
				set(c.x, c.y, c.z, s * 0.5f);
			}
		}

		quaternion& makeIdentity() 
		{
			x = y = z = 0;
			w = 1;
			return *this;
		}

		quaternion& set(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
			return *this;
		}

// 		matrix convertMatrix() 
// 		{
// 			matrix equalMatrix(false);
// 			equalMatrix(0, 0) = 1 - 2*(y*y + z*z);
// 			equalMatrix(1, 0) = 2*(x*y + w*z);
// 			equalMatrix(2, 0) = 2*(x*z - w*y);
// 			equalMatrix(0, 1) = 2*(x*y - w*z);
// 			equalMatrix(1, 1) = 1 - 2*(x*x + z*z);
// 			equalMatrix(2, 1) = 2*(y*z + w*x);
// 			equalMatrix(0, 2) = 2*(w*y + x*z);
// 			equalMatrix(1, 2) = 2*(y*z - w*x);
// 			equalMatrix(2, 2) = 1 - 2*(x*x + y*y);
// 			equalMatrix(0, 3) = equalMatrix(1, 3) = equalMatrix(2, 3) = 0;
// 			return equalMatrix;
// 		}

		bool operator!=(const quaternion& other) const
		{
			return x != other.x || y != other.y 
				|| z != other.z || w != other.w;
		}

		quaternion& operator*=(T scale)
		{
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
			return *this;
		}
		
		quaternion& operator+=(const quaternion& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.z;
		}

		quaternion& operator-=(const quaternion& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.z;
		}

		quaternion& operator*=(const quaternion& other)
		{
			float xbuf = x, ybuf = y, zbuf = z;

			x = ybuf*other.z - other.y*zbuf + w*other.x + other.w*xbuf;
			y = zbuf*other.x - other.z*xbuf + w*other.y + other.w*ybuf;
			z = xbuf*other.y - other.x*ybuf + w*other.z + other.w*zbuf;
			w = w*other.w - xbuf*other.x - ybuf*other.y - zbuf*other.z;

			return *this;
		}
	
		vector3d<T> operator*(const vector3d<T>& v) const
		{
			// nVidia SDK implementation
			// --> (*this) * quaternion(vector.x, vector.y, vector.z, 0) * this->conjugate().

			vector3d<T> qvec(x, y, z);
			vector3d<T> uv = qvec.cross(v);
			vector3d<T> uuv = qvec.cross(uv);
			uv *= (2.0f * w);
			uuv *= 2.0f;

			return ((uuv += uv) += v);
		}

		void toAngleAxis(radian_t<T>& angle, vector3d<T>& axis) const
		{
			axis.set(x, y, z);
			if (axis.isZero() || w > 1 || w < 1)
			{
				axis.set(0, 1, 0);
				angle = 0;
			} 
			else 
			{
				axis.nor();
				angle = 2 * ::math3d::acos(w);
			}
		}

		T len2() const
		{
			return x*x + y*y + z*z + w*w;
		}

		quaternion& normalize() 
		{
			T n = len2();

			if (n == 1.0f) 
				(*this) *= (1 / ::std::sqrt(n));

			return *this;
		}

		// @return the conjugate of this quaternion.
		quaternion conjugate() const
		{
			return quaternion(-x, -y, -z, w);
		}

		// @return the inverse of this quaternion, and inverse = conjugate / norm.
		//			if the quaternion's norm is 1, then inverse == conjugate.
		quaternion inverse() const
		{
			float scaler = 1 / len2();
		
			return quaternion(-x*scaler, -y*scaler, -z*scaler, w*scaler);
		}

		/************************************************************************/
		/* @param progress this must be ranging from 0 to 1.
		 *			if progress is 0, return quaternion that equals q1.
		 *			if progress is 1, return quaternion that equals q2.
		/************************************************************************/
		quaternion& lerp(const quaternion& q1, const quaternion& q2, float progress)
		{
			set(q2.x-q1.x , q2.y-q1.y, q2.z-q1.z, q2.w-q1.w);
			(*this) *= progress;
			(*this) += q1;
			return normalize();
		}

		quaternion& slerp(const quaternion& q1, const quaternion& q2, float progress, float threshold = 0.05f)
		{
			float cos = q1.dot(q2);	// think of quaternion as 4-vector.

			quaternion properQ1 = q1;
			if (cos < 0.0f)
			{
				properQ1 *= -1.0f;
				cos *= -1.0f;
			}

			if (cos <= (1-threshold)) // spherical interpolation
			{
				const float theta = acosf(cos);
				const float invsintheta = 1.f / (sinf(theta));
				const float scale = sinf(theta * (1.0f-progress)) * invsintheta;
				const float invscale = sinf(theta * progress) * invsintheta;
				return (*this = (properQ1*scale) + (q2*invscale));
			}
			else // linear interploation
			{
				return lerp(properQ1, q2, progress);
			}
		}
		
		float dot(const quaternion& other) const
		{
			return x*other.x + y*other.y + z*other.z + w*other.w;
		}

	private:
		float x, y, z, w;
	};	// end class quaternion.

}

#endif