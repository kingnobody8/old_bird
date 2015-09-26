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

		VIRTUAL const bool Line::IsParallel(const IDimOne* const test) const
		{
			assert(test);
			switch (test->GetType())
			{
			case EShape::LINE:
				return IsLineParallel(static_cast<const Line* const>(test));
				break;
			case EShape::RAY:
				return IsRayParallel(static_cast<const Ray* const>(test));
				break;
			case EShape::SEGMENT:
				return IsSegmentParallel(static_cast<const Segment* const>(test));
				break;
			}
			return false;
		}

		const bool Line::IsLineParallel(const Line* const test) const
		{
			return abs(dir.x) == abs(test->dir.x) && abs(dir.y) == abs(test->dir.y);
		}

		const bool Line::IsRayParallel(const Ray* const test) const
		{
			__todo()
				return false;
//			return abs(dir.x) == abs(test->dir)
		}

		const bool Line::IsSegmentParallel(const Segment* const test) const
		{
			__todo()
				return false;
		}

		VIRTUAL const bool Line::IsOrthoganal(const IDimOne* const test) const
		{
			__todo()
				return false;
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