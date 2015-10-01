#pragma once
#include "shape.h"

namespace util
{
	namespace shape
	{
		//forward declare
		class Ray;
		class Segment;

		class Line : public IDimOne
		{
		public:
			Line(); 
			virtual ~Line();
			virtual const float GetSlope() const { return dir.y / dir.x; };
			virtual bool Intersect(const math::vec2& point) const;

			void Create(const math::vec2& point_on_line, const math::vec2& direction);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

		private:
			const bool IsLineParallel(const Line* const test) const;
			const bool IsRayParallel(const Ray* const test) const;
			const bool IsSegmentParallel(const Segment* const test) const;
			math::vec2 pos;
			math::vec2 dir;
		};
	}
}