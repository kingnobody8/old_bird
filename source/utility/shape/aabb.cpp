#include "aabb.h"

namespace util
{
	namespace shape
	{
		STATIC const AABB AABB::INVALID_AABB = AABB(math::vec2(1,1), math::vec2(-1,-1));

		AABB::AABB()
		{
			m_eType = IShape::EShape::AABB;
		}

		AABB::AABB(const math::vec2& min, const math::vec2& max)
			: m_min(min)
			, m_max(max)
		{
		}

		VIRTUAL AABB::~AABB()
		{
		}

		VIRTUAL bool AABB::Intersect(const math::vec2& point) const
		{
			return point.x >= m_min.x && point.x <= m_max.x && point.y >= m_min.y && point.y <= m_max.y;
		}

		VIRTUAL const math::vec2 AABB::FindClosestPointOnEdge(const math::vec2& point) const
		{
			__not_implemented(AABB::FindClosestPointOnEdge);
			return math::vec2();
		}
	}
}