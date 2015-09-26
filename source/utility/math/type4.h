#pragma once
#include "types.h"
#include "math/type3.h"

namespace util
{
	namespace math
	{
		template<typename type>
		struct Type4
		{
			/*Data*/
		public:
			union	// Used to allow the data in the vector to be accessed in different ways
			{
				//Access the vector as an array
				type ary[4];

				//Access the vector's individual components
				struct // 3D Position
				{
					type x;
					type y;
					type z;
					type w;
				};
				struct // Color
				{
					type r;
					type g;
					type b;
					type a;
				};
				struct // Color Full
				{
					type red;
					type green;
					type blue;
					type alpha;
				};
				struct // HSV (not equivalent to RGB)
				{
					type hue;
					type sat;
					type val;
				};
			};

			/*Func*/
		public:
			Type4() : x(0), y(0), z(0), w(0){}
			Type4(const type &_x, const type &_y, const type &_z, const type&_w) : x(_x), y(_y), z(_z), w(_w) {}
			Type4(const Type3<type>& vec3, const type& _w = 0) : x(vec3.x), y(vec3.y), z(vec3.z), w(_w) {}

			//Operators
			inline Type4 operator+(const Type4 &v) const
			{
				return Type4(x + v.x, y + v.y, z + v.z, w + v.w);
			}
			inline Type4 operator-(const Type4 &v) const
			{
				return Type4(x - v.x, y - v.y, z - v.z, w - v.w);
			}
			inline Type4 operator*(const Type4 &v) const
			{
				return Type4(x * v.x, y * v.y, z * v.z, w * v.w);
			}
			inline Type4 operator/(const Type4 &v) const
			{
				return Type4(x / v.x, y / v.y, z / v.z.w / v.w);
			}
			//Special Operator
			inline Type4 operator*(type v) const
			{
				return Type4(x * v, y * v, z * v, w * v);
			}
			inline Type4 operator/(type v) const
			{
				return Type4(x / v, y / v, z / v, w / v);
			}

			//Comparisons
			inline bool operator==(const Type4 &v) const
			{
				return (EPSI(x, v.x) && EPSI(y, v.y) && EPSI(z, v.z) && EPSI(w, v.w));
			}
			inline bool operator!=(const Type4 &v) const
			{
				return !this->operator==(v);
			}

			//Assignment
			inline Type4& operator=(const Type4 &v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;
				return *this;
			}
			inline Type4& operator+=(const Type4 &v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
				return *this;
			}
			inline Type4& operator-=(const Type4 &v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
				return *this;
			}
			inline Type4& operator*=(const Type4 &v)
			{
				x *= v.x;
				y *= v.y;
				z *= v.z;
				w *= v.w;
				return *this;
			}
			inline Type4& operator/=(const Type4 &v)
			{
				x /= v.x;
				y /= v.y;
				z /= v.z;
				w /= v.w;
				return *this;
			}
			//Special Assignment
			inline Type4& operator*=(type v)
			{
				x *= v;
				y *= v;
				z *= v;
				w *= v;
				return *this;
			}
			inline Type4& operator/=(type v)
			{
				x /= v;
				y /= v;
				z /= v;
				w /= v;
				return *this;
			}

			//Special Member Funcs
			inline Type4& makeZero()
			{
				x = y = z = w = NULL;
				return *this;
			}
			inline Type4& negate()
			{
				x = -x;
				y = -y;
				z = -z;
				w = -w;
				return *this;
			}
			inline type dotProduct(const Type4& v) const
			{
				return x * v.x + y * v.y + z * v.z + w * v.w;
			}
			inline type magnitude() const
			{
				return (type)sqrt(dotProduct(*this));
			}
			inline type magnitudeSquared() const
			{
				return dotProduct(*this);
			}
			inline Type4& normalize()
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
			inline static Type4& Normalize(Type4 &v)
			{
				v.normalize();
				return v;
			}
			inline static type DotProduct(const Type4 &a, const Type4 &b)
			{
				return a.dotProduct(b);
			}
			inline static type Distance(const Type4& a, const Type4 &b)
			{
				return (a - b).magnitude();
			}
		};

		typedef Type4<float> vec4;
	}
}