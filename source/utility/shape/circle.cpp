#include "circle.h"
#include "macro.h"
#include <assert.h>
using namespace util::math;

namespace util
{
	namespace shape
	{
		Circle::Circle()
		{
			m_eType = IShape::EShape::CIRCLE;
		}
		
		VIRTUAL Circle::~Circle()
		{
		}

		VIRTUAL bool Circle::Intersect(const vec2& point) const
		{
			const vec2 diff = point - center;
			const float mag = diff.magnitude();
			return mag <= radius;
		}

		void Circle::Create(const vec2& c, const float& r)
		{
			center = c;
			radius = r;
		}

		VIRTUAL const math::vec2 Circle::FindClosestPointOnEdge(const math::vec2& point) const
		{
			vec2 dir = point - center;
			dir.normalize();
			const vec2 ret(center + dir * radius);
			return ret;
		}
	}
}