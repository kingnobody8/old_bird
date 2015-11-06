#include "line.h"
#include "macro.h"
#include "ray.h"
#include "segment.h"
#include <assert.h>
using namespace util::math;

namespace util
{
	namespace shape
	{
		Line::Line()
		{
			m_eType = IShape::EShape::LINE;
		}
		
		VIRTUAL Line::~Line()
		{
		}

		VIRTUAL bool Line::Intersect(const vec2& point) const
		{
			const vec2 v = point - pos; //direction of pos to point
			const float area = dir.x * v.y - dir.y * v.x; //cross product of the two vectors (dir & v) gives us area of triangle made by the line and the test point
			if (EPSI(area, 0.0f)) //if area is within epsilon of 0 then it lies on the line (close enough anyway)
				return true;
			return false;
		}

		void Line::Create(const vec2& point_on_line, const vec2& direction)
		{
			dir = vec2::Normalize(direction);
			pos = point_on_line;
			//set pos to closest point on line to origin
			pos = FindClosestPointOnEdge(vec2(0, 0));
		}

		VIRTUAL const math::vec2 Line::FindClosestPointOnEdge(const math::vec2& point) const
		{
			const vec2 v = point - pos;
			const float d = vec2::DotProduct(dir, v);
			const vec2 ret(dir * d);
			return ret;
		}
	}
}