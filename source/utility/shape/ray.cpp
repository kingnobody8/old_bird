#include "ray.h"
#include "macro.h"
using namespace util::math;

namespace util
{
	namespace shape
	{
		Ray::Ray()
		{
			m_eType = IShape::EShape::RAY;
		}
		
		VIRTUAL Ray::~Ray()
		{
		}
		
		VIRTUAL bool Ray::Intersect(const math::vec2& point) const
		{
			__not_implemented(Ray::Intersect);
			return false;
		}

		VIRTUAL bool Ray::Intersect(const IShape* const test) const
		{
			__not_implemented(Ray::Intersect);
			//__todo()//this isn't finished
			//const vec2 v = point - pos; //direction of pos to point
			//const float area = dir.x * v.y - dir.y * v.x; //cross product of the two vectors (dir & v) gives us area of triangle made by the line and the test point
			//if (!EPSI(area, 0.0f)) //if area is not within epsilon of 0 then it doesn't lie on th line
			//	return false;
			return true;
		}

		void Ray::Create(const vec2& start, const vec2& direction)
		{
			dir = vec2::Normalize(direction);
			pos = start;
		}

		VIRTUAL const math::vec2 Ray::FindClosestPointOnEdge(const math::vec2& point) const
		{
			__not_implemented(Ray::FindClosestPointOnEdge);
			//const vec2 v = point - pos;
			//const float d = vec2::DotProduct(dir, v);
			//const vec2 ret(dir * d);
			//return ret;
			return math::vec2();
		}
	}
}