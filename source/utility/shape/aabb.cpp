#include "aabb.h"
#include "segment.h"
#include "enums.h"
#include "func.h"

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

		void AABB::GetPoints(math::vec2 ary[4]) const
		{
			ary[0] = m_min;
			ary[1] = m_min + math::vec2(0, m_max.y);
			ary[2] = m_max;
			ary[4] = m_min + math::vec2(m_max.x, 0);
		}

		void AABB::MakeLimits()
		{
			__not_implemented(AABB::MakeLimits);
		}
		void AABB::Stretch(const math::vec2& point)
		{
			if (IsInvalid())
			{
				m_min = m_max = point;
				return;
			}
			m_min.x = Min(point.x, m_min.x);
			m_min.y = Min(point.y, m_min.y);
			m_max.x = Max(point.x, m_min.x);
			m_max.y = Max(point.y, m_min.y);
		}

		void AABB::Stretch(const AABB& box)
		{
			if (box.IsInvalid())
				return;

			Stretch(box.m_min);
			Stretch(box.m_max);
		}

		VIRTUAL bool AABB::Intersect(const math::vec2& point) const
		{
			return point.x >= m_min.x && point.x <= m_max.x && point.y >= m_min.y && point.y <= m_max.y;
		}

		VIRTUAL const math::vec2 AABB::FindClosestPointOnEdge(const math::vec2& point) const
		{
			math::vec2 points[4];
			GetPoints(points);

			Segment segs[EDirection::CARDINAL_DIRECTIONS_COUNT];
			segs[EDirection::LEFT] = Segment(points[0], points[1]);
			segs[EDirection::UP] = Segment(points[1], points[2]);
			segs[EDirection::RIGHT] = Segment(points[2], points[3]);
			segs[EDirection::DOWN] = Segment(points[3], points[0]);

			math::vec2 ret = points[0];
			for (int i = 0; i < EDirection::CARDINAL_DIRECTIONS_COUNT; ++i)
			{
				const math::vec2 tmp = segs[i].FindClosestPointOnEdge(point);
				if (math::vec2::Distance(tmp, point) < math::vec2::Distance(ret, point))
					ret = tmp;
			}

			return ret;
		}
	}
}