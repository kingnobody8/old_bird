#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeRect : public IRenderNode
		{
		private:
			util::shape::AABB m_aabb;
			bool m_fill = true;

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			virtual const b2PolygonShape& CalcShape();
			void SetAABB(const util::shape::AABB& aabb);
			inline const util::shape::AABB& GetAABB() const { return m_aabb; }
			inline void SetFill(const bool fill) { m_fill = fill; }
			inline const bool GetFill() const { return m_fill; }
		};
	}
}