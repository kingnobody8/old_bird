#pragma once
#include "types.h"
#include "math/consts.h"
#include <cmath>

#include "glm/glm.hpp"

namespace util
{
	namespace math
	{
		template<typename type>
		struct Type2
		{
			//TODO: figure out how to initalize a static const member of a templated class
			//static const Type2<type> ZERO_VECTOR;

			/*Data*/
		public:
			union	// Used to allow the data in the vector to be accessed in different ways
			{
				//Access the vector as an array
				type ary[2];

				//Access the vector's individual components
				struct //2D Position
				{
					type x;
					type y;
				};
				struct //2D Size
				{
					type w; // width
					type h; // height
				};
				struct //2D Texture
				{
					type u;
					type v;
				};
			};

			/*Func*/
		public:
			Type2():x(0), y(0){}
			Type2(const type &_x, const type &_y): x(_x), y(_y){}

			//Operators
			inline const Type2 operator+(const Type2 &v) const
			{
				return Type2(x + v.x, y + v.y);
			}
			inline const Type2 operator-(const Type2 &v) const
			{
				return Type2(x - v.x, y - v.y);
			}
			inline const Type2 operator*(const Type2 &v) const
			{
				return Type2(x * v.x, y * v.y);
			}
			inline const Type2 operator/(const Type2 &v) const
			{
				return Type2(x / v.x, y / v.y);
			}
			//Special Operator
			inline const Type2 operator*(type v) const
			{
				return Type2(x * v, y * v);
			}
			inline const Type2 operator/(type v) const
			{
				return Type2(x / v, y / v);
			}

			//Comparisons
			inline const bool operator==(const Type2 &v) const
			{
				return (EPSI(x, v.x) && EPSI(y, v.y));
			}
			inline const bool operator!=(const Type2 &v) const
			{
				return !this->operator==(v);
			}

			//Assignment
			inline Type2& operator=(const Type2 &v)
			{
				x = v.x;
				y = v.y;
				return *this;
			}
			inline Type2& operator+=(const Type2 &v)
			{
				x += v.x;
				y += v.y;
				return *this;
			}
			inline Type2& operator-=(const Type2 &v)
			{
				x -= v.x;
				y -= v.y;
				return *this;
			}
			inline Type2& operator*=(const Type2 &v)
			{
				x *= v.x;
				y *= v.y;
				return *this;
			}
			inline Type2& operator/=(const Type2 &v)
			{
				x /= v.x;
				y /= v.y;
				return *this;
			}
			//Special Assignment
			inline Type2& operator*=(type v)
			{
				x *= v;
				y *= v;
				return *this;
			}
			inline Type2& operator/=(type v)
			{
				x /= v;
				y /= v;
				return *this;
			}

			//Special Member Funcs
			inline Type2& makeZero()
			{
				x = y = NULL;
				return *this;
			}
			inline Type2& negate()
			{
				x = -x;
				y = -y;
				return *this;
			}
			inline type dotProduct(const Type2& v) const
			{
				return x * v.x + y * v.y;
			}
			inline type magnitude() const
			{
				return (type)sqrt(dotProduct(*this));
			}
			inline type magnitudeSquared() const
			{
				return dotProduct(*this);
			}
			inline Type2& normalize()
			{
				float invMag = 1 / magnitude();
				*this *= invMag;
				return *this;
			}
			inline bool& isZero() const
			{
				return this->x == 0 && this->y == 0;
			}

			//Static Funcs
			inline static Type2 Normalize(const Type2 &v)
			{
				float invMag = 1 / v.magnitude();
				Type2 ret = v * invMag;
				return ret;
			}
			inline static type DotProduct(const Type2 &a, const Type2 &b)
			{
				return a.dotProduct(b);
			}
			inline static type Distance(const Type2& a, const Type2 &b)
			{
				return (a - b).magnitude();
			}
		};

		typedef Type2<float> vec2;
	}
}
