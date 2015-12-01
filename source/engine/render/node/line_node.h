#pragma once
#include "render_node.h"
#include "shape/segment.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeLine : public IRenderNode
		{
		private:
			util::shape::Segment m_seg;

		protected:
			virtual const b2PolygonShape& CalcShape();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			
			void SetLine(const util::shape::Segment& seg);
		
			inline const util::shape::Segment& GetLine() const { return m_seg; }
		};
	}
}