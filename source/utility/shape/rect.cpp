#include "rect.h"

namespace util
{
	namespace shape
	{
		Rect::Rect()
		{
			m_eType = IShape::EShape::RECT;
		}

		Rect::Rect(const math::vec2& p, const math::vec2& s)
			: pos(p)
			, size(s)
		{
		}

		VIRTUAL Rect::~Rect()
		{
		}

		VIRTUAL bool Rect::Intersect(const math::vec2& point) const
		{
			return point.x >= pos.x && point.x <= pos.x + size.w && point.y >= pos.y && point.y <= pos.y + size.h;
		}

		VIRTUAL const math::vec2 Rect::FindClosestPointOnEdge(const math::vec2& point) const
		{
			__not_implemented(Rect::FindClosestPointOnEdge);
			return math::vec2();
		}
	}
}