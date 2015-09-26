#include "engine/render/render_node.h"
#include "engine/util/func.h"
#include "engine/engine.h"
#include "renderer.h"

namespace Engine
{
	using namespace Math;
	namespace Render
	{
		void IRenderNode::Register(const std::string& layer)
		{
			CRenderLayer* temp = CRenderer::Get()->GetLayer(layer);

			if (temp == null)
				return;

			temp->RegisterNode(this);
			this->m_pLayer = temp;
		}
		void IRenderNode::Unregister(void)
		{
			if (this->m_pLayer)
			{
				this->m_pLayer->UnregisterNode(this);
				this->m_pLayer = null;
			}
		}
		void IRenderNode::SetMatrix(const Math::CMatrix2D& mat)
		{
			position = mat.GetPosition();
			scale = mat.GetScale();
			rotation = mat.GetRotationZ();
		}
		void IRenderNode::SetZed(const float& zed)
		{
			position.z = zed;
		}
		const Math::CMatrix2D IRenderNode::GetMatrix(void) const
		{
			CMatrix2D ret;
			ret.SetPosition(vec2(position.x, position.y));
			ret.SetScale(scale);
			ret.SetRotationZ(rotation);
			return ret;
		}

		IRenderNode::IRenderNode(void)
			: m_pLayer(null)
		{
		}

		void CRenderNodeSprite::operator() (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam)
		{
			//TODO do real camera inversion to all properties, for now we just use the position
			//Math::Type3<float>	temp_position = position;
			//Math::Type2<float>	temp_scale = scale;
			//float				temp_rotation = rotation;
			vec2 cam_position = inv_cam.GetPosition();

			//Convert to screen coords
			Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			Type2<float> origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			Type2<float> screen_pos(origin.x + position.x + cam_position.x, origin.y + position.y + cam_position.y);

			//Set Blend mode
			SDL_SetTextureBlendMode(this->texture, this->blend_mode);
			//Set Alpha mod
			SDL_SetTextureAlphaMod(this->texture, this->clr.a);
			//Set Color mod
			SDL_SetTextureColorMod(this->texture, this->clr.r, this->clr.g, this->clr.b);

			//Set Src and Dst rect
			SDL_Rect srcRect;
			SDL_Rect dstRect;
			Uint32 format;
			int access;
			srcRect.x = srcRect.y = 0;
			SDL_QueryTexture(this->texture, &format, &access, &srcRect.w, &srcRect.h);
			dstRect.x = (int)round(screen_pos.x - srcRect.w * this->anchor.x * scale.x);
			dstRect.y = (int)round(screen_pos.y - srcRect.h * this->anchor.y * scale.y);
			
			//TODO this is for scissor rects
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
			
			//Convert to cartesian coords
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

		void CRenderNodeLine::operator() (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam)
		{
			//TODO do real camera inversion to all properties, for now we just use the position
			vec2 cam_position = inv_cam.GetPosition();

			//Convert to screen coords
			Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			Type2<float> origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			Type2<float> v1_screen_pos = origin + v1 + cam_position;
			Type2<float> v2_screen_pos = origin + v2 + cam_position;

			SDL_SetRenderDrawColor(pRen, clr.r, clr.g, clr.b, clr.a);
			SDL_RenderDrawLine(pRen, (int)v1_screen_pos.x, (int)(logical_size.y - v1_screen_pos.y), (int)v2_screen_pos.x, (int)(logical_size.y - v2_screen_pos.y));
		}

		void CRenderNodeRect::operator() (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam)
		{
			//TODO do real camera inversion to all properties, for now we just use the position
			vec2 cam_position = inv_cam.GetPosition();
			Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			Type2<float> origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
			vec2 pos = origin + vec2(dstRect.x, dstRect.y) + cam_position;
			dstRect.x = pos.x;
			dstRect.y = pos.y;

			SDL_SetRenderDrawColor(pRen, this->clr.r, this->clr.g, this->clr.b, this->clr.a);
			if (this->fill)
				SDL_RenderFillRect(pRen, &this->dstRect);
			else
				SDL_RenderDrawRect(pRen, &this->dstRect);
		}
	}
}