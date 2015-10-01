#pragma once
#include "line.h"

namespace util
{
	namespace shape
	{
		class Ray : public IDimOne
		{
		public:
			Ray();
			virtual ~Ray();

			virtual const float GetSlope() const { return dir.y / dir.x; }

			virtual bool Intersect(const math::vec2& point) const;
			virtual bool Intersect(const IShape* const test) const;

			void Create(const math::vec2& start, const math::vec2& direction);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

		private:
			math::vec2 pos;
			math::vec2 dir;
		};
	}
}