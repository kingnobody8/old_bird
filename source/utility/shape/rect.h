#pragma once
#include "shape.h"

namespace util
{
	namespace shape
	{
		class Rect : public IShape
		{
		public:
			Rect(void);
			Rect(const math::vec2& p, const math::vec2& s);
			Rect(const float& x, const float& y, const float& w, const float& h);
			virtual ~Rect();
			virtual bool Intersect(const math::vec2& point) const;

			void Create(const math::vec2& p, const math::vec2& s);
			virtual const math::vec2 FindClosestPointOnEdge(const math::vec2& point) const;

			union
			{
				float ary[4];
				struct
				{
					float x;
					float y;
					float w;
					float h;
				};
				struct
				{
					math::vec2 pos;
					math::vec2 size;
				};
			};

		
		};


	/*	class AABB : public IShape
		{
		public:
			Type2<float> min;
			Type2<float> max;
			const bool& GetValid() const { return m_valid; }
		private:
			bool		m_valid;

		public:
			AABB(bool valid = true)
				: m_valid(valid)
			{
			}
			AABB(Type2<float> min, Type2<float> max)
			{
				this->min = min;
				this->max = max;
			}
			void MakeLimits(void)
			{
				min = vec2(b2_maxFloat, b2_maxFloat);
				max = vec2(-b2_maxFloat, -b2_maxFloat);
			}
			const float GetRadius() const
			{
				return sqrt(pow(max.x - min.x, 2) + pow(max.y - min.y, 2));
			}
			const float GetWidth() const
			{
				return max.x - min.x;
			}
			const float GetHeight() const
			{
				return max.y - min.y;
			}
			const vec2 GetSize() const
			{
				return vec2(max.x - min.x, max.y - min.y);
			}
			const vec2 GetExtends() const
			{
				return vec2((max.x - min.x) / 2.0f, (max.y - min.y) / 2.0f);
			}
			void Extend(const AABB& that)
			{
				min.x = Min(min.x, that.min.x);
				min.y = Min(min.y, that.min.y);
				max.x = Max(max.x, that.max.x);
				max.y = Max(max.y, that.max.y);
			}
		};

		inline bool operator ==(const AABB& lhs, const AABB& rhs){ return (lhs.min.x == rhs.min.x && lhs.min.y == rhs.min.y && lhs.max.x == rhs.max.x && lhs.max.y == rhs.max.y); }
		inline bool operator !=(const AABB& lhs, const AABB& rhs){ return !operator==(lhs, rhs); }

		static AABB ConvertRectToAabb(const CRect& rect)
		{
			return AABB(rect.position, rect.position + rect.size);
		}
		static CRect ConvertAabbToRect(const AABB& aabb)
		{
			return CRect(aabb.min, aabb.max - aabb.min);
		}
		static b2PolygonShape ConvertAabbTob2PolygonShape(const AABB& aabb)
		{
			b2PolygonShape shape;
			shape.SetAsBox((aabb.max.x - aabb.min.x) / 2.0f, (aabb.max.y - aabb.min.y) / 2.0f);
			return shape;
		}
		static AABB Convertb2PolygonShapeToAabb(const b2PolygonShape& shape)
		{
			AABB aabb;
			b2AABB temp;
			b2Transform identity;
			identity.SetIdentity();
			shape.ComputeAABB(&temp, identity, 0);
			aabb.min = vec2(temp.lowerBound.x, temp.lowerBound.y);
			aabb.max = vec2(temp.upperBound.x, temp.upperBound.y);
			return aabb;
		}
		*/
	}
}