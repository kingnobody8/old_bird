#include "segment.h"
#include "macro.h"
using namespace util::math;

namespace util
{
	namespace shape
	{
		Segment::Segment()
		{
			m_eType = IShape::EShape::SEGMENT;
		}

		VIRTUAL Segment::~Segment()
		{
		}

		VIRTUAL const bool Segment::IsParallel(const IDimOne* const test) const
		{
			__not_implemented(Segment::IsParallel);
			return false;
		}
		VIRTUAL const bool Segment::IsOrthoganal(const IDimOne* const test) const
		{
			__not_implemented(Segment::IsOrthoganal);
			return false;
		}
		VIRTUAL const bool Segment::Intersect(const IDimOne* const test, math::vec2* result) const
		{
			__not_implemented(Segment::Intersect);
			return false;
		}


		VIRTUAL bool Segment::Intersect(const vec2& point) const
		{
			//const vec2 v = point - pos; //direction of pos to point
			//const float area = dir.x * v.y - dir.y * v.x; //cross product of the two vectors (dir & v) gives us area of triangle made by the line and the test point
			//if (!EPSI(area, 0.0f)) //if area is not within epsilon of 0 then it doesn't lie on th line
			//	return false;
			__not_implemented(Segment::Intersect);
			return true;
		}

		void Segment::Create(const math::vec2& s, const math::vec2& e)
		{
			start = s;
			end = e;
		}

		VIRTUAL const math::vec2 Segment::FindClosestPointOnEdge(const math::vec2& point) const
		{
			__not_implemented(Segment::FindClosestPointOnEdge);
			return math::vec2();

			//__todo()//this isn't finished
			//const vec2 v = point - pos;
			//const float d = vec2::DotProduct(dir, v);
			//const vec2 ret(dir * d);
			//return ret;
		}
	}
}