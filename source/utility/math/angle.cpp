#include "angle.h"

namespace util
{
	namespace math
	{
		Angle::Angle() 
			: m_radians(0)
		{
		}

		Angle::Angle(const float& rad)
			: m_radians(rad)
		{
		}

		void Angle::Normalize360()
		{
			m_radians = Normalize360(m_radians);
		}
		void Angle::Normalize180()
		{
			m_radians = Normalize180(m_radians);
		}

		Angle Angle::operator-(const Angle& other) const
		{
			return Delta(*this, other);
		}
		Angle& Angle::operator-=(const Angle& other)
		{
			return *this = *this - other;
		}
		Angle Angle::operator+(const Angle& other) const
		{
			return WrapRadians(m_radians + other.m_radians);
		}
		Angle& Angle::operator+=(const Angle& other)
		{
			return *this = *this + other;
		}
		
		STATIC float Angle::WrapRadians(float radians)
		{
			// bring into a range of -2PI,2PI
			radians = fmod(radians, math::TWO_PI);

			// bring into a range of -PI,PI
			if (radians > math::PI)
			{
				radians -= math::TWO_PI;
			}
			else if (radians < -math::PI)
			{
				radians += math::TWO_PI;
			}

			return radians;
		}
		STATIC Angle Angle::Delta(const Angle& a, const Angle& b)
		{
			float d = a.m_radians - b.m_radians;
			return Normalize180(d);
		}
		STATIC float Angle::Normalize360(float rads)
		{
			if (rads >= math::TWO_PI || rads < 0.0f)
			{
				rads -= floor(rads / math::TWO_PI) * math::TWO_PI;
			}
			return rads;
		}
		STATIC float Angle::Normalize180(float rads)
		{
			rads = Normalize360(rads);
			if (rads > math::PI)
			{
				rads -= math::TWO_PI;
			}
			return rads;
		}
	}
}