#pragma once
#include "shape.h"

namespace util
{
	namespace shape
	{
		class Circle : public IDimTwo
		{
		public:
			Circle();
			virtual ~Circle();
			virtual bool Intersect(const math::vec2& point) const;

			void Create(const math::vec2& c, const float& r);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

		private:
			math::vec2 center;
			float radius;
		};
	}
}