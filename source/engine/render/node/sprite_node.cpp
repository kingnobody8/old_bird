#include "sprite_node.h"
#include "../render_layer.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		CRenderNodeSprite::CRenderNodeSprite()
			: m_pTexture(null)
			, m_flip(SDL_RendererFlip::SDL_FLIP_NONE)
			, m_blend_mode(SDL_BlendMode::SDL_BLENDMODE_BLEND)
			, m_anchor(0.5f, 0.5f)
		{
		}

		VIRTUAL const b2PolygonShape& CRenderNodeSprite::CalcShape()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);

				Uint32 format;
				int access;
				util::math::Type2<int> size;
				SDL_QueryTexture(this->m_pTexture, &format, &access, &size.w, &size.h);

				const util::math::vec2 position = m_matrix.GetPosition();
				const util::math::vec2 scale = m_matrix.GetScale();
				const float rotation = m_matrix.GetRotationZ();

				//we multiply size by half because we want the extends, not the full size
				m_shape.SetAsBox((float32)(size.x * scale.x * 0.5f), (float32)(size.y * scale.y * 0.5f),
					b2Vec2((float32)position.x, (float32)position.y),
					(float32)(rotation * DEG_TO_RAD));
			}
			return m_shape;
		}
		
		VIRTUAL void CRenderNodeSprite::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			__todo() //do real camera inversion to all properties, for now we just use the position (subtracting position is a hack, do real matrix math)
				//Math::Type3<float>	temp_position = position;
				//Math::Type2<float>	temp_scale = scale;
				//float				temp_rotation = rotation;
				util::math::vec2 cam_position = inv_cam.GetPosition();

			util::math::vec2 position = m_matrix.GetPosition();
			util::math::vec2 scale = m_matrix.GetScale();
			float rotation = m_matrix.GetRotationZ();


			//Convert to screen coords
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			util::math::vec2 screen_pos(origin.x + position.x + cam_position.x, origin.y + position.y + cam_position.y);

			//Set Blend mode
			SDL_SetTextureBlendMode(this->m_pTexture, this->m_blend_mode);
			//Set Alpha mod
			SDL_SetTextureAlphaMod(this->m_pTexture, this->m_clr.a);
			//Set Color mod
			SDL_SetTextureColorMod(this->m_pTexture, this->m_clr.r, this->m_clr.g, this->m_clr.b);

			//Set Src and Dst rect
			SDL_Rect srcRect;
			SDL_Rect dstRect;
			Uint32 format;
			int access;
			srcRect.x = srcRect.y = 0;
			SDL_QueryTexture(this->m_pTexture, &format, &access, &srcRect.w, &srcRect.h);
			dstRect.x = (int)round(screen_pos.x - srcRect.w * this->m_anchor.x * scale.x);
			dstRect.y = (int)round(screen_pos.y - srcRect.h * this->m_anchor.y * scale.y);

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
			center.x = (int)round(this->m_anchor.x * dstRect.w);
			center.y = (int)round(this->m_anchor.y * dstRect.h);

			//Convert from cartesian coords to screen coords
			dstRect.y = logical_size.h - dstRect.y - dstRect.h;
			rotation = DEGREE_360 - rotation;
			center.y = dstRect.h - center.y;

			if (EPSI(rotation, DEGREE_360) || EPSI(rotation, 0.0f))
				rotation = 0.0f;

			ScissorOperation(pRen, origin);

			SDL_RenderCopyEx(pRen, this->m_pTexture, &srcRect, &dstRect, rotation, &center, this->m_flip);

			/*CRenderNodeRect rect;
			rect.SetAABB(this->CalcAABB());
			rect.SetFill(false);
			rect.SetScissorRect(m_scissor);
			rect(pRen, inv_cam);*/

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
	}
}