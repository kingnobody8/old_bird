#pragma once
#include "shape/aabb.h"
#include "shape/rect.h"
#include "shape/segment.h"
#include "math/type3.h"
#include "math/matrix_2d.h"
#include "sdl/include/SDL.h"
#include "bit_flag.h"
#include <list>
#include <map>
#include "macro.h"
#include "Collision/Shapes/b2PolygonShape.h"
#include "Collision/Shapes/b2EdgeShape.h"
#include "Collision/b2Collision.h"

#define ORIGIN_IS_CENTER = 1;

namespace engine
{
	namespace render
	{
		//Forward Declare
		class CRenderLayer;

		struct SpriteDesc
		{
			SDL_Texture*		texture;
			SDL_RendererFlip	flip;
			SDL_Color			clr;
			SDL_Rect			src_rect;
			SDL_BlendMode		blend_mode;
			util::math::vec2	anchor;
			float				realAlpha;
		};

		class IRenderNode
		{
		public:
			enum NodeStateFlag { 
				MOVE_DIRTY = 0x01,
				CULL_DIRTY = 0x02,
			};

		protected:
			CRenderLayer*		m_pLayer;
			b2PolygonShape		m_shape;
			util::Flag08		m_flag;
			SDL_Color			m_clr;
			float				m_zed;

		public:
			IRenderNode(void);
			virtual ~IRenderNode() = 0;
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam) = 0;
			virtual const b2PolygonShape& CalcShape() = 0;
			const util::shape::AABB CalcAABB();
			const bool CheckInView(const b2PolygonShape& view);
			inline void SetColor(const SDL_Color& clr) { m_clr = clr; }
			inline void SetZed(const float& zed) { m_zed = zed; }
			inline const SDL_Color& GetColor() const { return m_clr; }
			inline CRenderLayer* GetLayer() const { return m_pLayer; }
			inline const float& GetZed() const { return m_zed; }

			void Register(const std::string& layer);
			void Unregister(void);
		};

		class CRenderNodeSprite : public IRenderNode
		{
		private:
			util::math::Matrix2D m_matrix;
			util::math::vec2	m_anchor;
			SDL_Texture*		m_pTexture;
			SDL_RendererFlip	m_flip;
			SDL_BlendMode		m_blend_mode;
			__todo() // add source rect


		public:
			CRenderNodeSprite();
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			virtual const b2PolygonShape& CalcShape();

			inline void SetMatrix(const util::math::Matrix2D& mat) { m_matrix = mat; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetAnchor(const util::math::vec2& anchor) { m_anchor = anchor; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetTexture(SDL_Texture* const tex) { m_pTexture = tex; }
			inline void SetFlip(const SDL_RendererFlip& flip) { m_flip = flip; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetBlendMode(const SDL_BlendMode& blend) { m_blend_mode = blend; }
			
			inline const util::math::Matrix2D& GetMatrix() { return m_matrix; }
			inline const util::math::vec2& GetAnchor() { return m_anchor; }
			inline SDL_Texture* const GetTexture() const { return m_pTexture; }
			inline const SDL_RendererFlip& GetFlip() const { return m_flip; }
			inline const SDL_BlendMode& GetBlendMode() const { return m_blend_mode; }
		};

		__todo() //probably this should contain a list of sprites that it allocates & deallocates as it needs them
		//class CRenderNodeText : public IRenderNode
		//{
		//	//TODO
		//public:
		//	virtual void operator () (SDL_Renderer* pRen, CMatrix2D camera);
		//};

		class CRenderNodeLine : public IRenderNode
		{
		private:
			util::shape::Segment m_seg;

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			virtual const b2PolygonShape& CalcShape();
			void SetLine(const util::shape::Segment& seg);
			inline const util::shape::Segment& GetLine() const { return m_seg; }
		};

		class CRenderNodeRect : public IRenderNode
		{
		private:
			util::shape::AABB m_aabb;
			bool m_fill = true;

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);
			virtual const b2PolygonShape& CalcShape();
			void SetAABB(const util::shape::AABB& aabb);
			inline const util::shape::AABB& GetAABB() const { return m_aabb; }
			inline void SetFill(const bool fill) { m_fill = fill; }
			inline const bool GetFill() const { return m_fill; }
		};
	}
}