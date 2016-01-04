#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeLine : public IRenderNode
		{
		private:
			b2EdgeShape m_seg;

		protected:
			virtual void CalcAabbInternal();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			
			void SetLine(const b2EdgeShape& seg);
		
			inline const b2EdgeShape& GetLine() const { return m_seg; }
		};
	}
}