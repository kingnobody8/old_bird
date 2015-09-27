#include "matrix_2d.h"
#include <cmath>

namespace util
{
	namespace math
	{
		float sign(float f)
		{
			return (f >= 0) ? 1.0f : -1.0f;
		}

		Matrix2D::Matrix2D(void) //default is identity matrix
			: _e11(1), _e12(0), _e13(0)
			, _e21(0), _e22(1), _e23(0)
			, _e31(0), _e32(0), _e33(1)
		{
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}

		Matrix2D::Matrix2D(const float(&ary)[9])
			: _e11(ary[0]), _e12(ary[1]), _e13(ary[2])
			, _e21(ary[3]), _e22(ary[4]), _e23(ary[5])
			, _e31(ary[6]), _e32(ary[7]), _e33(ary[8])
		{
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}

		Matrix2D::Matrix2D(const vec2& pos, const vec2& scale, const float& rot)
		{
			//Set the scale
			*this = Matrix_Create_Scale(scale.x, scale.y);
			//Set the rotation
			*this *= Matrix_Create_Rotation_Z(rot);
			//Set the position
			this->SetPosition(pos);
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}

		Matrix2D::Matrix2D(const float& e11, const float& e12, const float& e13,
			const float& e21, const float& e22, const float& e23,
			const float& e31, const float& e32, const float& e33)
			: _e11(e11), _e12(e12), _e13(e13)
			, _e21(e21), _e22(e22), _e23(e23)
			, _e31(e31), _e32(e32), _e33(e33)
		{
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}

#if MATRIX_DEBUG
		void Matrix2D::CacheVals()
		{
			this->pos = GetPosition();
			this->scale = GetScale();
			this->rot = GetRotationZ();
		}
#endif

		void Matrix2D::SetPosition(const Type2<float>& pos)
		{
			this->SetPosition(pos.x, pos.y);
		}
		void Matrix2D::SetPosition(const float& x, const float& y)
		{
			this->_e13 = x;
			this->_e23 = y;
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}
		void Matrix2D::SetPositionX(const float& x)
		{
			this->_e13 = x;
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}
		void Matrix2D::SetPositionY(const float& y)
		{
			this->_e23 = y;
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}
		void Matrix2D::SetScale(const Type2<float>& scale)
		{
			this->SetScale(scale.x, scale.y);
		}
		void Matrix2D::SetScale(const float& x, const float& y)
		{
			//Save pos and rotation
			Type2<float> pos = this->GetPosition();
			float rot = this->GetRotationZ();
			//Set the scale
			*this = Matrix_Create_Scale(x, y);
			//Set the rotation
			*this *= Matrix_Create_Rotation_Z(rot);
			//Set the position
			this->SetPosition(pos);
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}
		void Matrix2D::SetRotationZ(const float& rot)
		{
			//Save pos and scale
			Type2<float> pos = this->GetPosition();
			Type2<float> scale = this->GetScale();
			//Set the scale
			*this = Matrix_Create_Scale(scale.x, scale.y);
			//Set the rotation
			*this *= Matrix_Create_Rotation_Z(rot);
			//Set the position
			this->SetPosition(pos);
#if MATRIX_DEBUG
			this->CacheVals();
#endif
		}

		Type2<float> Matrix2D::GetPosition(void) const
		{
			return Type2<float>(this->_e13, this->_e23);
		}
		Type2<float> Matrix2D::GetScale(void) const
		{
			Type2<float> ret;
			float sig = sign(Matrix_Determinant(*this));
			ret.x = sig * sqrt(pow(this->_e11, 2) + pow(this->_e12, 2));
			ret.y = sig * sqrt(pow(this->_e21, 2) + pow(this->_e22, 2));
			return ret;
		}
		float Matrix2D::GetRotationZ(void) const
		{
			return atan2(-this->_e12, this->_e11) * RAD_TO_DEG;
		}

		//Operators
		Matrix2D Matrix2D::operator+ (const Matrix2D &v) const
		{
			return Matrix_Matrix_Add(*this, v);
		}
		Matrix2D Matrix2D::operator-(const Matrix2D &v) const
		{
			return Matrix_Matrix_Sub(*this, v);
		}
		Matrix2D Matrix2D::operator*(const Matrix2D &v) const
		{
			return Matrix_Matrix_Multiply(*this, v);
		}
		//Special Operator
		Matrix2D Matrix2D::operator*(float v) const
		{
			return Matrix_Scalar_Multiply(*this, v);
		}
		Matrix2D Matrix2D::operator/(float v) const
		{
			return Matrix_Scalar_Multiply(*this, 1 / v);
		}
		Matrix2D& Matrix2D::operator *= (const Matrix2D &v)
		{
			*this = Matrix_Matrix_Multiply(*this, v);
			return *this;
		}


		STATIC const Matrix2D Matrix2D::Matrix_Zero(void)
		{
			Matrix2D m;
			for (int i = 0; i < 9; i++)
				m.e[i] = 0;
			return m;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Identity(void)
		{
			Matrix2D m = Matrix_Zero();
			m._e11 = m._e22 = m._e33 = 1;
			return m;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Create_Translation(const float& x, const float& y)
		{
			const float ary[] =
			{
				1, 0, x,
				0, 1, y,
				0, 0, 1,
			};
			const Matrix2D m(ary);
			return m;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Create_Translation(const Type2<float>& val)
		{
			return Matrix_Create_Translation(val.x, val.y);
		}
		STATIC const Matrix2D Matrix2D::Matrix_Create_Scale(const float& x, const float& y)
		{
			const float ary[] =
			{
				x, 0, 0,
				0, y, 0,
				0, 0, 1,
			};
			const Matrix2D m(ary);
			return m;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Create_Scale(const Type2<float>& val)
		{
			return Matrix_Create_Scale(val.x, val.y);
		}
		STATIC const Matrix2D Matrix2D::Matrix_Create_Rotation_Z(const float& Deg)
		{
			const float rad = Deg * DEG_TO_RAD;
			const float ary[] =
			{
				float(cos(rad)), float(-sin(rad)), 0,
				float(sin(rad)), float(cos(rad)), 0,
				0, 0, 1,
			};
			const Matrix2D m(ary);
			return m;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Matrix_Add(const Matrix2D& m, const Matrix2D& n)
		{
			Matrix2D ret;
			for (int i = 0; i < 9; i++)
				ret.e[i] = m.e[i] + n.e[i];
			return ret;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Matrix_Sub(const Matrix2D& m, const Matrix2D& n)
		{
			Matrix2D ret;
			for (int i = 0; i < 9; i++)
				ret.e[i] = m.e[i] - n.e[i];
			return ret;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Scalar_Multiply(const Matrix2D& m, const float& s)
		{
			Matrix2D ret;
			for (int i = 0; i < 16; i++)
				ret.e[i] = m.e[i] * s;
			return ret;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Negate(const Matrix2D& m)
		{
			Matrix2D ret;
			for (int i = 0; i < 16; i++)
				ret.e[i] = -m.e[i];
			return ret;
		}
		STATIC const Matrix2D Matrix2D::Matrix_Transpose(const Matrix2D& m)
		{
			const float ary[] =
			{
				m._e11, m._e21, m._e31,
				m._e12, m._e22, m._e32,
				m._e13, m._e23, m._e33,
			};

			const Matrix2D ret(ary);
			return ret;
		}
		// Multipy a matrix and a vector
		//
		// IN:		m		The matrix (left hand side)
		//			v		The vector (right hand side)
		//
		// RETURN:	[m]*v
		STATIC const vec2 Matrix2D::Vector_Matrix_Multiply(const vec2& v, const Matrix2D& m)
		{
			vec2 ret;
			ret.x = (v.x * m._e11) + (v.y * m._e12) + (1.0f * m._e13);
			ret.y = (v.x * m._e21) + (v.y * m._e22) + (1.0f * m._e23);
			return ret;
		}
		// Multiply a matrix by a matrix
		//
		// IN:		m		First Matrix (left hand side)
		//			n		Second Matrix (right hand side)
		//
		// RETURN:	[m]*[n]
		STATIC const Matrix2D Matrix2D::Matrix_Matrix_Multiply(const Matrix2D& m, const Matrix2D& n)
		{
			const float ary[] =
			{
				m._e11*n._e11 + m._e12*n._e21 + m._e13*n._e31,
				m._e11*n._e12 + m._e12*n._e22 + m._e13*n._e32,
				m._e11*n._e13 + m._e12*n._e23 + m._e13*n._e33,
				m._e21*n._e11 + m._e22*n._e21 + m._e23*n._e31,
				m._e21*n._e12 + m._e22*n._e22 + m._e23*n._e32,
				m._e21*n._e13 + m._e22*n._e23 + m._e23*n._e33,
				m._e31*n._e11 + m._e32*n._e21 + m._e33*n._e31,
				m._e31*n._e12 + m._e32*n._e22 + m._e33*n._e32,
				m._e31*n._e13 + m._e32*n._e23 + m._e33*n._e33
			};

			const Matrix2D ret(ary);
			return ret;
		}
		STATIC const float Matrix2D::Matrix_Determinant(const Matrix2D& m)
		{
			return Determinant_3x3(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e31, m._e32, m._e33);
		}
		STATIC const Matrix2D Matrix2D::Matrix_Inverse(const Matrix2D& m)
		{
			const float tempDet = Matrix_Determinant(m);
			if (EPSI(tempDet, 0))
			{
				return m;
			}

			const float ary[] =
			{
				powf(-1, 1 + 1)*Determinant_2x2(m._e22, m._e23, m._e32, m._e33),
				powf(-1, 1 + 2)*Determinant_2x2(m._e21, m._e23, m._e31, m._e33),
				powf(-1, 1 + 3)*Determinant_2x2(m._e21, m._e22, m._e31, m._e32),
				powf(-1, 2 + 1)*Determinant_2x2(m._e12, m._e13, m._e32, m._e33),
				powf(-1, 2 + 2)*Determinant_2x2(m._e11, m._e13, m._e31, m._e33),
				powf(-1, 2 + 3)*Determinant_2x2(m._e11, m._e12, m._e31, m._e32),
				powf(-1, 3 + 1)*Determinant_2x2(m._e12, m._e13, m._e22, m._e23),
				powf(-1, 3 + 2)*Determinant_2x2(m._e11, m._e13, m._e21, m._e23),
				powf(-1, 3 + 3)*Determinant_2x2(m._e11, m._e12, m._e21, m._e22)
			};

			Matrix2D ret(ary);
			//transpose it
			ret = Matrix_Transpose(ret);
			for (int i = 0; i < 9; i++)
			{
				ret.e[i] = ret.e[i] / tempDet;
			}
			return ret;
		}
		STATIC const float Matrix2D::Determinant_2x2(float e_11, float e_12, float e_21, float e_22)
		{
			return e_11 * e_22 - e_12 * e_21;
		}
		STATIC const float Matrix2D::Determinant_3x3(float e11, float e12, float e13, float e21, float e22, float e23, float e31, float e32, float e33)
		{
			return e11*(e22*e33 - e32*e23) - e12*(e21*e33 - e31*e23) + e13*(e21*e32 - e31*e22);
		}

		//STATIC const Matrix2D Matrix2D::Matrix_Vector_Multiply(const Matrix2D& m, const vec2& v){}
	}
}