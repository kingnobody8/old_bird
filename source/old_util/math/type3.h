#pragma once
#include "types.h"
#include "math/type2.h"

namespace util
{
	namespace math
	{
		template<typename type>
		struct Type3
		{
			/*Data*/
		public:
			union	// Used to allow the data in the vector to be accessed in different ways
			{
				//Access the vector as an array
				type ary[3];

				//Access the vector's individual components
				struct //3D Position
				{
					type x;
					type y;
					type z;
				};
				struct //3D Size
				{
					type w;	// width
					type h; // height
					type d; // depth
				};
			};

			/*Func*/
		public:
			Type3() : x(0), y(0), z(0){}
			Type3(const type &_x, const type &_y, const type &_z) : x(_x), y(_y), z(_z) {}
			Type3(const Type2<type>& vec2, const type& _z = 0) : x(vec2.x), y(vec2.y), z(_z) {}

			//Operators
			inline Type3 operator+(const Type3 &v) const
			{
				return Type3(x + v.x, y + v.y, z + v.z);
			}
			inline Type3 operator-(const Type3 &v) const
			{
				return Type3(x - v.x, y - v.y, z - v.z);
			}
			inline Type3 operator*(const Type3 &v) const
			{
				return Type3(x * v.x, y * v.y, z * v.z);
			}
			inline Type3 operator/(const Type3 &v) const
			{
				return Type3(x / v.x, y / v.y, z / v.z);
			}
			//Special Operator
			inline Type3 operator*(type v) const
			{
				return Type3(x * v, y * v, z * v);
			}
			inline Type3 operator/(type v) const
			{
				return Type3(x / v, y / v, z / v);
			}

			//Comparisons
			inline bool operator==(const Type3 &v) const
			{
				return (EPSI(x, v.x) && EPSI(y, v.y) && EPSI(z, v.z));
			}
			inline bool operator!=(const Type3 &v) const
			{
				return !this->operator==(v);
			}

			//Assignment
			inline Type3& operator=(const Type3 &v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				return *this;
			}
			inline Type3& operator+=(const Type3 &v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				return *this;
			}
			inline Type3& operator-=(const Type3 &v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
				return *this;
			}
			inline Type3& operator*=(const Type3 &v)
			{
				x *= v.x;
				y *= v.y;
				z *= v.z;
				return *this;
			}
			inline Type3& operator/=(const Type3 &v)
			{
				x /= v.x;
				y /= v.y;
				z /= v.z;
				return *this;
			}
			//Special Assignment
			inline Type3& operator*=(type v)
			{
				x *= v;
				y *= v;
				z *= v;
				return *this;
			}
			inline Type3& operator/=(type v)
			{
				x /= v;
				y /= v;
				z /= v;
				return *this;
			}

			//Special Funcs
			inline Type3& makeZero()
			{
				x = y = z = NULL;
				return *this;
			}
			inline Type3& negate()
			{
				x = -x;
				y = -y;
				z = -z;
				return *this;
			}
			inline type dotProduct(const Type3 &v) const
			{
				return x * v.x + y * v.y + z * v.z;
			}
			inline type magnitude() const
			{
				return (type)sqrt(dotProduct(*this));
			}
			inline type magnitudeSquared() const
			{
				return dotProduct();
			}
			inline Type3& normalize()
			{
				float invMag = 1 / magnitude();
				*this *= invMag;
				return *this;
			}

			//Static Funcs
			inline static Type3& Normalize(Type3 &v)
			{
				v.normalize();
				return v;
			}
			inline static type DotProduct(const Type3 &a, const Type3 &b)
			{
				return a.dotProduct(b);
			}
			inline static type Distance(const Type3 &a, const Type3 &b)
			{
				return (a - b).magnitude();
			}
		};

		typedef Type3<float> vec3;
	}
}