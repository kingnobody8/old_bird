#include "rect_node.h"
#include "../render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		void CRenderNodeRect::SetAABB(const util::shape::AABB& aabb)
		{
			this->m_aabb = aabb;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;

		}
	
		VIRTUAL const b2PolygonShape& CRenderNodeRect::CalcShape()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);

				m_shape.SetAsBox((float32)m_aabb.CalcExtends().x, (float32)m_aabb.CalcExtends().h,
					b2Vec2((float32)m_aabb.CalcCenter().x, (float32)m_aabb.CalcCenter().y), 0);
			}
			return m_shape;
		}
		
		VIRTUAL void CRenderNodeRect::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			//because sdl only renders rects as aabb, rotation cannot be applied, therefore we will only be using the position and scale part of the inv_cam
			const util::math::Matrix2D test = util::math::Matrix2D(inv_cam.GetPosition(), inv_cam.GetScale(), 0.0f);
			util::shape::AABB tmp = m_aabb;

			//get screen space
			tmp.m_min = util::math::Matrix2D::Vector_Matrix_Multiply(tmp.m_min, test);
			tmp.m_max = util::math::Matrix2D::Vector_Matrix_Multiply(tmp.m_max, test);

			//get screen info
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			//set the sdl rect
			SDL_Rect rect;
			rect.x = (int)(origin.x + tmp.m_min.x);
			rect.y = (int)(origin.y - tmp.m_max.y);
			rect.w = (int)tmp.CalcSize().w;
			rect.h = (int)tmp.CalcSize().h;

			ScissorOperation(pRen, origin);

			SDL_SetRenderDrawColor(pRen, this->m_clr.r, this->m_clr.g, this->m_clr.b, this->m_clr.a);
			if (this->m_fill)
				SDL_RenderFillRect(pRen, &rect);
			else
				SDL_RenderDrawRect(pRen, &rect);
		}
	}
}