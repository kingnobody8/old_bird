#include "point_node.h"
#include "../render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		void CRenderNodePoint::SetPoint(const util::math::vec2& point)
		{
			this->m_point = point;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}
		VIRTUAL const b2PolygonShape& CRenderNodePoint::CalcShape()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);
				m_shape.SetAsBox(0, 0, b2Vec2(m_point.x, m_point.y), 0); __todo() // is this valid?
			}
			return m_shape;
		}
		VIRTUAL void CRenderNodePoint::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			//get screen space
			util::math::vec2 tmp = util::math::Matrix2D::Vector_Matrix_Multiply(m_point, inv_cam);

			//get screen info
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			//set the sdl points
			SDL_Point p;
			p.x = (int)(origin.x + tmp.x);
			p.y = (int)(origin.y - tmp.y);

			ScissorOperation(pRen, origin);

			SDL_SetRenderDrawColor(pRen, m_clr.r, m_clr.g, m_clr.b, m_clr.a);
			SDL_RenderDrawPoint(pRen, p.x, p.y);
		}
	}
}