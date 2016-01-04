#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeRect : public IRenderNode
		{
		private:
			b2AABB m_aabb;
			bool m_fill = true;

		protected:
			virtual const b2PolygonShape& CalcShape();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			
			void SetAABB(const b2AABB& aabb);
			inline void SetFill(const bool fill) { m_fill = fill; }

			inline const b2AABB& GetAABB() const { return m_aabb; }
			inline const bool GetFill() const { return m_fill; }
		};
	}
}