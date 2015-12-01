#include "render_node.h"
#include "../render_layer.h"
#include "func.h"
#include "sprite_node.h"

namespace engine
{
	namespace render
	{
		void IRenderNode::Register(const std::string& layer)
		{
			Unregister();

			CRenderLayer* tmp = CRenderLayer::FindLayer(layer);
			assert(tmp);

			tmp->RegisterNode(this);
			this->m_pLayer = tmp;
		}
		
		void IRenderNode::Unregister(void)
		{
			if (this->m_pLayer)
			{
				this->m_pLayer->UnregisterNode(this);
				this->m_pLayer = null;
			}
		}

		IRenderNode::IRenderNode(void)
			: m_pLayer(null)
			, m_flag(MOVE_DIRTY | CULL_DIRTY)
			, m_zed(0.0f)
			, m_use_scissor(false)
		{
			m_clr.a = m_clr.r = m_clr.g = m_clr.b = 255;
		}
		
		VIRTUAL IRenderNode::~IRenderNode()
		{
			Unregister();
		}
		
		void IRenderNode::SetScissorRect(const util::shape::AABB& scissor)
		{
			if (scissor.IsInvalid())
				return;
			m_scissor = scissor;
			m_use_scissor = true;
		}
		
		void IRenderNode::ScissorOperation(SDL_Renderer* pRen, const util::math::vec2& origin)
		{
			if (m_use_scissor)
			{
				SDL_Rect scissor;
				scissor.x = (int)(origin.x + m_scissor.m_min.x);
				scissor.y = (int)(origin.y - m_scissor.m_max.y);
				scissor.w = (int)m_scissor.CalcSize().w;
				scissor.h = (int)m_scissor.CalcSize().h;
				SDL_RenderSetClipRect(pRen, &scissor);
			}
			else
			{
				SDL_RenderSetClipRect(pRen, null);
			}
		}
		
		const bool IRenderNode::CheckInView(const b2PolygonShape& view)
		{
			__todo()
				//USE BOX2D polygonshape for shape checks, add a function to each rendernode that returns it's aabb,
				//each rendernode should then cache its aabb, only changing it if it moves
				//we can use this for fast query of aabb on parts too by asking their render nodes
				//since the nodes store the aabb, they can use it for culling
				//we probably will have to perform a full cull each frame (that means checking every node aabb against the camera aabb)
				//however one idea is to cache a bool saying if an object has moved since last cull, it has then we only check the objects that have moved
				//howeve against that idea is that if the camera moves we still have to recheck everything, so this won't be very useful during gameplay

				CalcShape();

			b2Transform trans_shape, trans_view;
			const bool overlap = b2TestOverlap(&m_shape, 0, &view, 0, trans_shape, trans_view);

			return overlap;
		}
		
		const util::shape::AABB IRenderNode::CalcAABB()
		{
			m_shape = CalcShape();
			b2AABB temp;
			b2Transform identity;
			identity.SetIdentity();
			m_shape.ComputeAABB(&temp, identity, 0);
			const util::math::vec2 min(temp.lowerBound.x, temp.lowerBound.y);
			const util::math::vec2 max(temp.upperBound.x, temp.upperBound.y);
			const util::shape::AABB aabb(min, max);
			return aabb;
		}
	}
}