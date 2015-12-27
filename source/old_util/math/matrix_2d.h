#pragma once
#include "type2.h"

#ifdef _DEBUG
#define MATRIX_DEBUG 1
#endif
namespace util
{
	namespace math
	{
		struct Matrix2D //Column Matrix
		{
#if MATRIX_DEBUG
			vec2 pos;
			vec2 scale;
			float rot;
			void CacheVals();
#endif

			union
			{
				// NOTE - first 3 are _e11 - _e13
				float e[9];

				// Single elements in row major format
				struct
				{
					float _e11, _e12, _e13;
					float _e21, _e22, _e23;
					float _e31, _e32, _e33;
				};
				/*
				[	x_scale * cos@,		sin@,				x_pos	]
				[	-sin@,				y_scale * cos@,		y_pos	]
				[		0,				0,					1		]
				*/
			};

			Matrix2D(void);
			Matrix2D(const float (&ary)[9]); //array of 9 floats
			Matrix2D(const vec2& pos, const vec2& scale, const float& rot);
			Matrix2D(const float& e11, const float& e12, const float& e13,
				const float& e21, const float& e22, const float& e23,
				const float& e31, const float& e32, const float& e33);

			void SetPosition(const vec2& pos);
			void SetPosition(const float& x, const float& y);
			void SetPositionX(const float& x);
			void SetPositionY(const float& y);
			void SetScale(const Type2<float>& scale);
			void SetScale(const float& x, const float& y);
			void SetRotationZ(const float& rot);

			Type2<float> GetPosition(void) const;
			Type2<float> GetScale(void) const;
			float		 GetRotationZ(void) const;

			//Operators
			Matrix2D operator+(const Matrix2D &v) const;
			Matrix2D operator-(const Matrix2D &v) const;
			Matrix2D operator*(const Matrix2D &v) const;
			//Special Operator
			Matrix2D operator*(float v) const;
			Matrix2D operator/(float v) const;
			Matrix2D& operator*=(const Matrix2D &v);

			static const Matrix2D	Matrix_Zero(void);
			static const Matrix2D	Matrix_Identity(void);
			static const Matrix2D	Matrix_Create_Translation(const float& x, const float& y);
			static const Matrix2D	Matrix_Create_Translation(const Type2<float>& val);
			static const Matrix2D	Matrix_Create_Scale(const float& x, const float& y);
			static const Matrix2D	Matrix_Create_Scale(const Type2<float>& val);
			static const Matrix2D	Matrix_Create_Rotation_Z(const float& Deg);
			static const Matrix2D	Matrix_Matrix_Add(const Matrix2D& m, const Matrix2D& n);
			static const Matrix2D	Matrix_Matrix_Sub(const Matrix2D& m, const Matrix2D& n);
			static const Matrix2D	Matrix_Scalar_Multiply(const Matrix2D& m, const float& s);
			static const Matrix2D	Matrix_Negate(const Matrix2D& m);
			static const Matrix2D	Matrix_Transpose(const Matrix2D& m);
			static const vec2		Vector_Matrix_Multiply(const vec2& v, const Matrix2D& m);
			static const Matrix2D	Matrix_Matrix_Multiply(const Matrix2D& m, const Matrix2D& n);
			static const float		Matrix_Determinant(const Matrix2D& m);
			static const Matrix2D	Matrix_Inverse(const Matrix2D& m);
			static const float		Determinant_2x2(float e_11, float e_12, float e_21, float e_22);
			static const float		Determinant_3x3(float e_11, float e_12, float e_13, float e_21, float e_22, float e_23, float e_31, float e_32, float e_33);
			//static const Matrix2D	Matrix_Vector_Multiply(const Matrix2D& m, const vec2& v);
		};
	}
}