#pragma once
#include "line.h"

namespace util
{
	namespace shape
	{
		class Segment : public IDimOne
		{
		public:
			Segment();
			Segment(const math::vec2 s, const math::vec2 e);
			virtual ~Segment();

			inline math::vec2 GetDirection() const { return math::vec2::Normalize(end - start); }
			virtual const float GetSlope() const { return GetDirection().y / GetDirection().x; }

			virtual const bool Intersect(const IDimOne* const test, math::vec2* result = nullptr) const;
			virtual bool Intersect(const math::vec2& point) const;

			void Create(const math::vec2& s, const math::vec2& e);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

		public:
			math::vec2 start;
			math::vec2 end;
		};
	}
}