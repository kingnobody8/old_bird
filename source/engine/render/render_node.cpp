#include "render_node.h"
#include "render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		__todo()// go through and make all the render funcs use the top left corner as origin, so that we don't do the extra calculations
			//if we want something centered on screen then we should move the camera over it (-half_screen_width, -half_screen_height)
			//but for now maybe just put the origin offsets in a #debug statement

		void IRenderNode::Register(const std::string& layer)
		{
			__not_implemented(IRenderNode::Register);
			/*CRenderLayer* temp = CRenderer::Get()->GetLayer(layer);

			if (temp == null)
			return;

			temp->RegisterNode(this);
			this->m_pLayer = temp;*/
		}
		void IRenderNode::Unregister(void)
		{
			__not_implemented(IRenderNode::UnRegister);
			/*if (this->m_pLayer)
			{
			this->m_pLayer->UnregisterNode(this);
			this->m_pLayer = null;
			}*/
		}

		IRenderNode::IRenderNode(void)
			: m_pLayer(null)
			, m_flag(MOVE_DIRTY | CULL_DIRTY)
		{
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

		//SPRITE
		VIRTUAL const b2PolygonShape& CRenderNodeSprite::CalcShape()
		{
			if (m_flag.Bit(EFlag::MOVE_DIRTY))
			{
				m_flag.BitOff(EFlag::MOVE_DIRTY);

				Uint32 format;
				int access;
				util::math::Type2<int> size;
				SDL_QueryTexture(this->texture, &format, &access, &size.w, &size.h);

				//we multiply size by half because we want the extends, not the full size
				m_shape.SetAsBox((float32)(size.x * scale.x * 0.5f), (float32)(size.y * scale.y * 0.5f),
					b2Vec2((float32)position.x, (float32)position.y),
					(float32)(rotation * DEG_TO_RAD));
			}
			return m_shape;
		}
		void CRenderNodeSprite::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			__todo() //do real camera inversion to all properties, for now we just use the position (subtracting position is a hack, do real matrix math)
				//Math::Type3<float>	temp_position = position;
				//Math::Type2<float>	temp_scale = scale;
				//float				temp_rotation = rotation;
				util::math::vec2 cam_position = inv_cam.GetPosition();

			//Convert to screen coords
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			util::math::vec2 screen_pos(origin.x + position.x + cam_position.x, origin.y + position.y + cam_position.y);

			//Set Blend mode
			SDL_SetTextureBlendMode(this->texture, this->blend_mode);
			//Set Alpha mod
			SDL_SetTextureAlphaMod(this->texture, this->m_clr.a);
			//Set Color mod
			SDL_SetTextureColorMod(this->texture, this->m_clr.r, this->m_clr.g, this->m_clr.b);

			//Set Src and Dst rect
			SDL_Rect srcRect;
			SDL_Rect dstRect;
			Uint32 format;
			int access;
			srcRect.x = srcRect.y = 0;
			SDL_QueryTexture(this->texture, &format, &access, &srcRect.w, &srcRect.h);
			dstRect.x = (int)round(screen_pos.x - srcRect.w * this->anchor.x * scale.x);
			dstRect.y = (int)round(screen_pos.y - srcRect.h * this->anchor.y * scale.y);

			__todo() // this is for using only a portion of the texture when rendering (possibly scissor rects)
				if (false/*pSection != nullptr*/)
				{
					//srcRect = *(SDL_Rect*)(pSection);
					//dstRect.w = slong(pSection->w * fScale.x);
					//dstRect.h = slong(pSection->h * fScale.y);
				}
				else
				{
					srcRect.x = srcRect.y = 0;
					dstRect.w = (int)round(srcRect.w * scale.x);
					dstRect.h = (int)round(srcRect.h * scale.y);
				}

			SDL_Point center;
			center.x = (int)round(this->anchor.x * dstRect.w);
			center.y = (int)round(this->anchor.y * dstRect.h);

			//Convert from cartesian coords to screen coords
			dstRect.y = logical_size.h - dstRect.y - dstRect.h;
			rotation = DEGREE_360 - rotation;
			center.y = dstRect.h - center.y;

			if (EPSI(rotation, DEGREE_360) || EPSI(rotation, 0.0f))
				rotation = 0.0f;

			SDL_RenderCopyEx(pRen, this->texture, &src_rect, &dstRect, rotation, &center, this->flip);

			//TODO figure out how to make textures look brighter (whiten)
			//if (realAlpha > 1.0f)
			//{
			//	for (int i = 0; i < 20; ++i)
			//	{
			//		uchar a = Min(realAlpha * 255 - i * 255, 255);
			//
			//		//Set Blend mode
			//		SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_ADD);
			//		//Set Alpha mod
			//		SDL_SetTextureAlphaMod(this->texture, 255);
			//		//Set Color mod
			//		SDL_SetTextureColorMod(this->texture, this->clr.r, this->clr.g, this->clr.b);
			//		//Draw
			//		SDL_RenderCopyEx(pRen, this->texture, &src_rect, &dstRect, rotation, &center, this->flip);
			//	}
			//}
		}

		//LINE
		void CRenderNodeLine::SetLine(const util::shape::Segment& seg)
		{
			this->m_seg = seg;
			m_flag = EFlag::MOVE_DIRTY | EFlag::CULL_DIRTY;
		}
		VIRTUAL const b2PolygonShape& CRenderNodeLine::CalcShape()
		{
			if (m_flag.Bit(EFlag::MOVE_DIRTY))
			{
				m_flag.BitOff(EFlag::MOVE_DIRTY);

				util::shape::AABB box;
				box.m_min = util::math::vec2(Min(m_seg.start.x, m_seg.end.x), Min(m_seg.start.y, m_seg.end.y));
				box.m_max = util::math::vec2(Max(m_seg.start.x, m_seg.end.x), Max(m_seg.start.y, m_seg.end.y));

				m_shape.SetAsBox((float32)box.CalcExtends().x, (float32)box.CalcExtends().y,
					b2Vec2((float32)box.CalcCenter().x, (float32)box.CalcCenter().y), 0);
			}
			return m_shape;
		}
		void CRenderNodeLine::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
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

			SDL_SetRenderDrawColor(pRen, m_clr.r, m_clr.g, m_clr.b, m_clr.a);
			SDL_RenderDrawLine(pRen, p1.x, p1.y, p2.x, p2.y);
		}

		//RECT
		void CRenderNodeRect::SetAABB(const util::shape::AABB& aabb)
		{
			this->m_aabb = aabb;
			m_flag = EFlag::MOVE_DIRTY | EFlag::CULL_DIRTY;

		}
		VIRTUAL const b2PolygonShape& CRenderNodeRect::CalcShape()
		{
			if (m_flag.Bit(EFlag::MOVE_DIRTY))
			{
				m_flag.BitOff(EFlag::MOVE_DIRTY);

				m_shape.SetAsBox((float32)m_aabb.CalcExtends().x, (float32)m_aabb.CalcExtends().h,
				b2Vec2((float32)m_aabb.CalcCenter().x, (float32)m_aabb.CalcCenter().y), 0);
			}
			return m_shape;
		}
		void CRenderNodeRect::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
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

			SDL_SetRenderDrawColor(pRen, this->m_clr.r, this->m_clr.g, this->m_clr.b, this->m_clr.a);
			if (this->m_fill)
				SDL_RenderFillRect(pRen, &rect);
			else
				SDL_RenderDrawRect(pRen, &rect);
		}
	}
}