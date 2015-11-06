#pragma once
#include "shape.h"

namespace util
{
	namespace shape
	{
		class AABB : public IShape
		{
		public:
			static const AABB INVALID_AABB;

			AABB();
			AABB(const math::vec2& min, const math::vec2& max);
			virtual ~AABB();
			virtual bool Intersect(const math::vec2& point) const;
			bool IsInvalid() const { return m_min.x > m_max.x || m_min.y > m_max.y; }

			void GetPoints(math::vec2 ary[4]) const;

			void Create(const math::vec2& min, const math::vec2& max);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

			void MakeLimits();
			void Stretch(const math::vec2& point);
			void Stretch(const AABB& box);

			inline const math::vec2 CalcSize() const { return m_max - m_min; }
			inline const math::vec2 CalcExtends() const { return (m_max - m_min) * 0.5f; }
			inline const math::vec2 CalcCenter() const { return m_min + (m_max - m_min) * 0.5f; }

			math::vec2 m_min;
			math::vec2 m_max;
		};

	}
}