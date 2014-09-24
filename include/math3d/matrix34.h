#ifndef MATH3D_MATRIX_34_H
#define MATH3D_MATRIX_34_H
#include "vector4d.h"

namespace math3d
{
	/// Affine matrix44
	/**
	 * m2[0][0] m2[0][1] m2[0][2] m2[0][3]
	 * 
	 * m2[1][0] m2[1][1] m2[1][2] m2[1][3]
	 * 
	 * m2[2][0] m2[2][1] m2[2][2] m2[2][3]
	 * 
	 *    0        0        0        1
	 */
	template <typename T>
	class matrix34_t :
		boost::additive<matrix34_t<T>,
				boost::equality_comparable<matrix34_t<T> > >
	{
	private:
		typedef matrix34_t<T> MyType;
		typedef vector4d<T> Vec4;
		typedef vector3d<T> Vec3;
		
	public:
		union {
			T m1[12];
			T m2[3][4];
		};

	public:
		static const MyType IDENTITY;

	public:
		explicit matrix34_t()
		{
		}
		
		explicit matrix34_t(bool isIdentity)
		{
			if (isIdentity) 
				this->makeIdentity();
		}

		explicit matrix34_t(T* data)
		{
			memcpy(m1, data, sizeof(m1));
		}

		matrix34_t(const Vec4& row0, const Vec4& row1, const Vec4& row2)
		{
			setRows(row0, row1, row2);
		}

		void setTranslation(const Vec3& vec)
		{
			m1[0][3] = vec.x;
			m1[1][3] = vec.y;
			m1[2][3] = vec.z;
		}

		Vec3 getTranslation() const
		{
			return Vec3(m2[0][3], m2[1][3], m2[2][3]);
		}

		void setScale(const Vec3& scale)
		{
			m1[0][0] = scale.x;
			m1[1][1] = scale.y;
			m1[2][2] = scale.z;
		}

		Vec3 getAbsScale() const
		{
			return Vec3(getColAsVec3(0).len(),
						getColAsVec3(1).len(),
						getColAsVec3(2).len());
		}

		Vec4 operator*(const Vec4& vector) const 
		{
			return Vec4(getRow(0).dot(vector), 
						getRow(1).dot(vector), 
						getRow(2).dot(vector), 
						vector.w);
		}

		Vec3 transformVector(const Vec3& vector) const 
		{
			return Vec3(_cast_vec<vector3d>(0).dot(vector),
						_cast_vec<vector3d>(1).dot(vector),
						_cast_vec<vector3d>(2).dot(vector));
		}

		Vec3 transformPoint(const Vec3& point) const
		{
			Vec3 transformed = transformVector(point);
			return transformed += getColAsVec3(3);
		}

		MyType& operator*=(const MyType& other) 
		{
			Vec3 otherCols[4] = {
				other.getColAsVec3(0), other.getColAsVec3(1),
				other.getColAsVec3(2), other.getColAsVec3(3),
			};

			float* rowPtr;
			for (int row = 0; row < 3; ++row)
			{
				rowPtr = this->m2[row];

				// copy row because the row data will change in this block
				Vec3 thisRow(rowPtr);

				rowPtr[0] = thisRow.dot(otherCols[0]);
				rowPtr[1] = thisRow.dot(otherCols[1]);
				rowPtr[2] = thisRow.dot(otherCols[2]);

				rowPtr[3] += thisRow.dot(otherCols[3]);
			}

			return *this;
		}

		bool operator==(const MyType& other) const
		{
			return  _cast_vec<vector4d>(0) == other._cast_vec<vector4d>(0) &&
					_cast_vec<vector4d>(1) == other._cast_vec<vector4d>(1) &&
					_cast_vec<vector4d>(2) == other._cast_vec<vector4d>(2);
		}

		void makeScale(const Vec3& s)
		{
			m2[0][0] = s.x; m2[0][1] = 0;	m2[0][2] = 0;	m2[0][3] = 0; 

			m2[1][0] = 0;	m2[1][1] = s.y; m2[1][2] = 0;	m2[1][3] = 0; 

			m2[2][0] = 0;	m2[2][1] = 0;	m2[2][2] = s.z; m2[2][3] = 0;
		}

		void makeTranslation(const Vec3& t) 
		{
			m2[0][0] = 1; m2[0][1] = 0; m2[0][2] = 0; m2[0][3] = t.x; 

			m2[1][0] = 0; m2[1][1] = 1; m2[1][2] = 0; m2[1][3] = t.y; 

			m2[2][0] = 0; m2[2][1] = 0; m2[2][2] = 1; m2[2][3] = t.z;
		}

		inline void makeIdentity()
		{
			makeTranslation(Vec3::ZERO);
		}

		inline bool isIdentity() const
		{
			return (*this) == MyType::IDENTITY;
		}

		inline float trace() const
		{
			return m2[0][0] + m2[1][1] + m2[2][2] + 1;
		}

		inline bool getInverse(MyType& out) const
		{
			const Vec3& row0 = _cast_vec<vector3d>(0);
			const Vec3& row1 = _cast_vec<vector3d>(1);
			const Vec3& row2 = _cast_vec<vector3d>(2);

			Vec3 row1_cross_row2 = row1.cross(row2);
			T det = row0.dot(row1_cross_row2);
			if (det != 0)
			{
				T invDet = 1 / det;
				Vec3 invDet_x_row0 = row0 * invDet;

				out.setCols33(row1_cross_row2 *= invDet, 
							  row2.cross(invDet_x_row0), 
							  invDet_x_row0.cross(row1));

				out.setTranslation(-out.transfromVector(getTranslation()));
				return true;
			}
			
			return false;
		}

		inline Vec4 getCol(int col) const 
		{
			assert(col >= 0 && col < 4);
			T w = (col == 3) ? 1 : 0;
			return Vec3(m2[0][col], m2[1][col], m2[2][col], w);
		}

		inline Vec3 getColAsVec3(int col) const 
		{
			assert(col >= 0 && col < 4);
			return Vec3(m2[0][col], m2[1][col], m2[2][col]);
		}
		
		void setCols33(const Vec3& col0, const Vec3& col1, const Vec3& col2)
		{
			m2[0][0] = col0.x;
			m2[0][1] = col1.x;
			m2[0][2] = col2.x;

			m2[1][0] = col0.y;
			m2[1][1] = col1.y;
			m2[1][2] = col2.y;

			m2[2][0] = col0.z;
			m2[2][1] = col1.z;
			m2[2][2] = col2.z;
		}
		
		void setCols(const Vec3& col0, const Vec3& col1, 
					 const Vec3& col2, const Vec3& col3)
		{
			setCols33(col0, col1, col2);
			setTranslation(col3);
		}

		inline const Vec4& getRow(int row) const
		{
			assert(row >= 0 && row < 4);
			return row == 3 ? Vec4::UNIT_W : _cast_vec<vector4d>(row);
		}

		inline const Vec3& getRowAsVec3(int row) const
		{
			assert(row >= 0 && row < 4);
			return row == 3 ? Vec3::ZERO : _cast_vec<vector3d>(row);
		}
		
		void setRows(const Vec4& row0, const Vec4& row1, const Vec4& row2)
		{
			_cast_vec<vector4d>(0) = row0;
			_cast_vec<vector4d>(1) = row1;
			_cast_vec<vector4d>(2) = row2;
		}

	private:
		template <template <class> class Vector>
		inline const Vector<T>& _cast_vec(int row) const
		{
			assert(row >= 0 && row < 3);
			return *reinterpret_cast<const Vector<T>*> (m2[row]);
		}

		template <template <class> class Vector>
		inline Vector<T>& _cast_vec(int row) 
		{
			return *reinterpret_cast<Vector<T>*> (m2[row]);
		}
	};

	template <typename T>
	const matrix34_t<T> matrix34_t<T>::IDENTITY;


	typedef matrix34_t<float> mat34;
	typedef matrix34_t<double> dmat34;
}

#endif
