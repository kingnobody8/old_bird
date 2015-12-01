#include "circle_node.h"
#include "../render_layer.h"
#include "sprite_node.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		/*
		* This is a 32-bit pixel function created with help from this
		* website: http://www.libsdl.org/intro.en/usingvideo.html
		*
		* You will need to make changes if you want it to work with
		* 8-, 16- or 24-bit surfaces.  Consult the above website for
		* more information.
		*/
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
		{
			Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
			*(Uint32 *)target_pixel = pixel;
		}

		/*
		* This is an implementation of the Midpoint Circle Algorithm
		* found on Wikipedia at the following link:
		*
		*   http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
		*
		* The algorithm elegantly draws a circle quickly, using a
		* set_pixel function for clarity.
		*/
		void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel)
		{
			// if the first pixel in the screen is represented by (0,0) (which is in sdl)
			// remember that the beginning of the circle is not in the middle of the pixel
			// but to the left-top from it:

			double error = (double)-radius;
			double x = (double)radius - 0.5;
			double y = (double)0.5;
			double cx = n_cx - 0.5;
			double cy = n_cy - 0.5;

			while (x >= y)
			{
				set_pixel(surface, (int)(cx + x), (int)(cy + y), pixel);
				set_pixel(surface, (int)(cx + y), (int)(cy + x), pixel);

				if (x != 0)
				{
					set_pixel(surface, (int)(cx - x), (int)(cy + y), pixel);
					set_pixel(surface, (int)(cx + y), (int)(cy - x), pixel);
				}

				if (y != 0)
				{
					set_pixel(surface, (int)(cx + x), (int)(cy - y), pixel);
					set_pixel(surface, (int)(cx - y), (int)(cy + x), pixel);
				}

				if (x != 0 && y != 0)
				{
					set_pixel(surface, (int)(cx - x), (int)(cy - y), pixel);
					set_pixel(surface, (int)(cx - y), (int)(cy - x), pixel);
				}

				error += y;
				++y;
				error += y;

				if (error >= 0)
				{
					--x;
					error -= x;
					error -= x;
				}
			}
		}

		/*
		* SDL_Surface 32-bit circle-fill algorithm without using trig
		*
		* While I humbly call this "Celdecea's Method", odds are that the
		* procedure has already been documented somewhere long ago.  All of
		* the circle-fill examples I came across utilized trig functions or
		* scanning neighbor pixels.  This algorithm identifies the width of
		* a semi-circle at each pixel height and draws a scan-line covering
		* that width.
		*
		* The code is not optimized but very fast, owing to the fact that it
		* alters pixels in the provided surface directly rather than through
		* function calls.
		*
		* WARNING:  This function does not lock surfaces before altering, so
		* use SDL_LockSurface in any release situation.
		*/
		void fill_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel)
		{
			// Note that there is more to altering the bitrate of this 
			// method than just changing this value.  See how pixels are
			// altered at the following web page for tips:
			//   http://www.libsdl.org/intro.en/usingvideo.html
			static const int BPP = 4;

			double r = (double)radius;

			for (double dy = 1; dy <= r; dy += 1.0)
			{
				// This loop is unrolled a bit, only iterating through half of the
				// height of the circle.  The result is used to draw a scan line and
				// its mirror image below it.

				// The following formula has been simplified from our original.  We
				// are using half of the width of the circle because we are provided
				// with a center and we need left/right coordinates.

				double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
				int x = cx - dx;

				// Grab a pointer to the left-most pixel for each half of the circle
				Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
				Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

				for (; x <= cx + dx; x++)
				{
					*(Uint32 *)target_pixel_a = pixel;
					*(Uint32 *)target_pixel_b = pixel;
					target_pixel_a += BPP;
					target_pixel_b += BPP;
				}
			}
		}

		void CRenderNodeCircle::SetCenter(const util::math::vec2& center)
		{
			m_center = center;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}
		
		void CRenderNodeCircle::SetRadius(const float& radius)
		{
			m_radius = radius;
			m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY;
		}
		
		VIRTUAL const b2PolygonShape& CRenderNodeCircle::CalcShape()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);

				util::shape::AABB box;
				box.m_min = m_center - util::math::vec2(m_radius, m_radius);
				box.m_max = m_center + util::math::vec2(m_radius, m_radius);

				m_shape.SetAsBox((float32)box.CalcExtends().x, (float32)box.CalcExtends().y,
					b2Vec2((float32)box.CalcCenter().x, (float32)box.CalcCenter().y), 0);
			}
			return m_shape;
		}
		
		VIRTUAL void CRenderNodeCircle::operator() (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam)
		{
			__todo()// find out if this is really slow
				//it seems like it might be because i am creating a new render surface, but maybe not


				SDL_Surface *surface;
			Uint32 rmask, gmask, bmask, amask;
			int width = m_radius * 2;  __todo()// why do i have to multiply by 2 twice?
				int height = m_radius * 2;

			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
			on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
#endif
			surface = SDL_CreateRGBSurface(0, width * 2, height * 2, 32, rmask, gmask, bmask, amask);

			if (surface == null)
				util::CheckSdlError();

			Uint32 pixel;
			memcpy(&pixel, &m_clr, sizeof(Uint32));

			if (m_fill)
				fill_circle(surface, width, height, m_radius, pixel);
			else
				draw_circle(surface, width, height, m_radius, pixel);

			SDL_Texture* tex = SDL_CreateTextureFromSurface(pRen, surface);

			util::math::Matrix2D mat;
			mat.SetPosition(m_center);
			CRenderNodeSprite sprite;
			sprite.SetBlendMode(SDL_BlendMode::SDL_BLENDMODE_BLEND);
			sprite.SetColor(m_clr);
			sprite.SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
			sprite.SetTexture(tex);
			sprite.SetMatrix(mat);
			sprite(pRen, inv_cam);
			//SDL_RenderCopyEx(pRen, tex, null, null, 0, null, SDL_FLIP_NONE);
		}
	}
}