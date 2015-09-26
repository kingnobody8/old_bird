#pragma once
#include "math/type2.h"
#include "macro.h"

namespace util
{
	namespace shape
	{
		class IShape
		{
		public:
			//NOTE point is not included because vec2 is used and a point is not a shape
			// also even though rect and aabb are technically polygons, they are special because their calculations are much simplier than a free form polygon
			enum EShape { INVALID, LINE, SEGMENT, RAY, CIRCLE, RECT, AABB, POLYGON, SHAPE_COUNT }; 
		protected:
			EShape m_eType;
		public:
			IShape() : m_eType(INVALID){}
			virtual ~IShape() = 0;
			
			bool Intersect(const IShape* const test) const;
			virtual bool Intersect(const math::vec2& point) const = 0;
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const = 0;

			//Gets
			EShape GetType(void) const { return m_eType; }
		};

		class IDimOne : public IShape //this is for one dimensional shapes [ LINE, SEGMENT, & RAY ]
		{
			virtual const bool IsParallel(const IDimOne* const test) const = 0;
			virtual const bool IsOrthoganal(const IDimOne* const test) const = 0;
			const bool IsOblique(const IDimOne* const test) const { return !IsParallel(test) && !IsOrthoganal(test); }
			virtual const bool Intersect(const IDimOne* const test, math::vec2* result = nullptr) const = 0;
		};

		class IDimTwo : public IShape //this is for two dimensional shapes [ RECT, AABB, CIRCLE, & POLYGON ]
		{
		public:
			virtual const float CalcArea() const = 0;
			virtual const math::vec2 CalcCenter() const = 0;
			virtual const math::vec2 CalcExtends() const = 0;
		};
	}
}