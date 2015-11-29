#include "render_node.h"
#include "render_layer.h"
#include "func.h"
#include "render_layer.h"

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

		//SPRITE
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

		//POINT
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

		//LINE
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

		//CIRCLE
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

			SDL_Surface *surface;
			Uint32 rmask, gmask, bmask, amask;
			int width = m_radius * 2;
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

		//RECT
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
			////because sdl only renders rects as aabb, rotation cannot be applied, therefore we will only be using the position and scale part of the inv_cam
			//const util::math::Matrix2D test = util::math::Matrix2D(inv_cam.GetPosition(), inv_cam.GetScale(), 0.0f);
			//util::shape::AABB tmp = m_aabb;

			////get screen space
			//tmp.m_min = util::math::Matrix2D::Vector_Matrix_Multiply(tmp.m_min, test);
			//tmp.m_max = util::math::Matrix2D::Vector_Matrix_Multiply(tmp.m_max, test);

			////get screen info
			//util::math::Type2<int> logical_size;
			//SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			//util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			////set the sdl rect
			//SDL_Rect rect;
			//rect.x = (int)(origin.x + tmp.m_min.x);
			//rect.y = (int)(origin.y - tmp.m_max.y);
			//rect.w = (int)tmp.CalcSize().w;
			//rect.h = (int)tmp.CalcSize().h;

			//ScissorOperation(pRen, origin);

			//SDL_SetRenderDrawColor(pRen, this->m_clr.r, this->m_clr.g, this->m_clr.b, this->m_clr.a);
			//if (this->m_fill)
			//	SDL_RenderFillRect(pRen, &rect);
			//else
			//	SDL_RenderDrawRect(pRen, &rect);

			CRenderNodeCircle circle;
			circle.SetCenter(m_aabb.CalcCenter());
			circle.SetRadius(m_aabb.CalcExtends().x);
			circle.SetColor(m_clr);
			circle(pRen, inv_cam);
		}
	}
}