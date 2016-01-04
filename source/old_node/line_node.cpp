#include "line_node.h"
#include "../render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		void CRenderNodeLine::SetLine(const b2EdgeShape& seg)
		{
			this->m_seg = seg;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}
		
		VIRTUAL void CRenderNodeLine::CalcAabbInternal()
		{
			b2Transform identity;
			identity.SetIdentity();
			m_seg.ComputeAABB(&m_aabb, identity, 0);
		}
		
		VIRTUAL void CRenderNodeLine::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			//util::math::vec2 tmp1 = m_seg.start;
			//util::math::vec2 tmp2 = m_seg.end;

			////get screen space
			//tmp1 = util::math::Matrix2D::Vector_Matrix_Multiply(tmp1, inv_cam);
			//tmp2 = util::math::Matrix2D::Vector_Matrix_Multiply(tmp2, inv_cam);

			////get screen info
			//util::math::Type2<int> logical_size;
			//SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			//util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			////set the sdl points
			//SDL_Point p1, p2;
			//p1.x = (int)(origin.x + tmp1.x);
			//p1.y = (int)(origin.y - tmp1.y);
			//p2.x = (int)(origin.x + tmp2.x);
			//p2.y = (int)(origin.y - tmp2.y);

			//ScissorOperation(pRen, origin);

			//SDL_SetRenderDrawColor(pRen, m_clr.r, m_clr.g, m_clr.b, m_clr.a);
			//SDL_RenderDrawLine(pRen, p1.x, p1.y, p2.x, p2.y);
		}
	}
}