#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodePoint : public IRenderNode
		{
		private:
			util::math::vec2 m_point;

		protected:
			virtual const b2PolygonShape& CalcShape();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			
			void SetPoint(const util::math::vec2& seg);
			
			inline const util::math::vec2& GetPoint() const { return m_point; }
		};
	}
}