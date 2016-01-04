#include "circle_node.h"
#include "../render_layer.h"
#include "sprite_node.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		void CRenderNodeCircle::SetCircle(const b2CircleShape& circle)
		{
			m_circle = circle;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}

		VIRTUAL void CRenderNodeCircle::CalcAabbInternal()
		{
			b2Transform identity;
			identity.SetIdentity();
			m_circle.ComputeAABB(&m_aabb, identity, 0);
		}

		VIRTUAL void CRenderNodeCircle::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
//			__todo()// find out if this is really slow
//				//it seems like it might be because i am creating a new render surface, but maybe not
//
//
//				SDL_Surface *surface;
//			Uint32 rmask, gmask, bmask, amask;
//			int width = m_radius * 2;  __todo()// why do i have to multiply by 2 twice?
//				int height = m_radius * 2;
//
//			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
//			on the endianness (byte order) of the machine */
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//			rmask = 0xff000000;
//			gmask = 0x00ff0000;
//			bmask = 0x0000ff00;
//			amask = 0x000000ff;
//#else
//			rmask = 0x000000ff;
//			gmask = 0x0000ff00;
//			bmask = 0x00ff0000;
//			amask = 0xff000000;
//#endif
//			surface = SDL_CreateRGBSurface(0, width * 2, height * 2, 32, rmask, gmask, bmask, amask);
//
//			if (surface == null)
//				util::CheckSdlError();
//
//			Uint32 pixel;
//			memcpy(&pixel, &m_clr, sizeof(Uint32));
//
//			if (m_fill)
//				fill_circle(surface, width, height, m_radius, pixel);
//			else
//				draw_circle(surface, width, height, m_radius, pixel);
//
//			SDL_Texture* tex = SDL_CreateTextureFromSurface(pRen, surface);
//
//			util::math::Matrix2D mat;
//			mat.SetPosition(m_center);
//			CRenderNodeSprite sprite;
//			sprite.SetBlendMode(SDL_BlendMode::SDL_BLENDMODE_BLEND);
//			sprite.SetColor(m_clr);
//			sprite.SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
//			sprite.SetTexture(tex);
//			sprite.SetMatrix(mat);
//			sprite(pRen, inv_cam);
//			//SDL_RenderCopyEx(pRen, tex, null, null, 0, null, SDL_FLIP_NONE);
		}
	}
}