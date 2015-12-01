#include "line_node.h"
#include "../render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		void CRenderNodeLine::SetLine(const util::shape::Segment& seg)
		{
			this->m_seg = seg;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}
		
		VIRTUAL const b2PolygonShape& CRenderNodeLine::CalcShape()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);

				util::shape::AABB box;
				box.m_min = util::math::vec2(Min(m_seg.start.x, m_seg.end.x), Min(m_seg.start.y, m_seg.end.y));
				box.m_max = util::math::vec2(Max(m_seg.start.x, m_seg.end.x), Max(m_seg.start.y, m_seg.end.y));

				m_shape.SetAsBox((float32)box.CalcExtends().x, (float32)box.CalcExtends().y,
					b2Vec2((float32)box.CalcCenter().x, (float32)box.CalcCenter().y), 0);
			}
			return m_shape;
		}
		
		VIRTUAL void CRenderNodeLine::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			util::math::vec2 tmp1 = m_seg.start;
			util::math::vec2 tmp2 = m_seg.end;

			//get screen space
			tmp1 = util::math::Matrix2D::Vector_Matrix_Multiply(tmp1, inv_cam);
			tmp2 = util::math::Matrix2D::Vector_Matrix_Multiply(tmp2, inv_cam);

			//get screen info
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			//set the sdl points
			SDL_Point p1, p2;
			p1.x = (int)(origin.x + tmp1.x);
			p1.y = (int)(origin.y - tmp1.y);
			p2.x = (int)(origin.x + tmp2.x);
			p2.y = (int)(origin.y - tmp2.y);

			ScissorOperation(pRen, origin);

			SDL_SetRenderDrawColor(pRen, m_clr.r, m_clr.g, m_clr.b, m_clr.a);
			SDL_RenderDrawLine(pRen, p1.x, p1.y, p2.x, p2.y);
		}
	}
}