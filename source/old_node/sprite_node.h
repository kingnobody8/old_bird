#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeSprite : public IRenderNode
		{
		private:
			util::math::Matrix2D	m_matrix;
			util::math::vec2		m_anchor;
			
			SDL_Texture*			m_pTexture;
			SDL_RendererFlip		m_flip;
			SDL_BlendMode			m_blend_mode;
			__todo() // add source rect

		protected:
			virtual const b2PolygonShape& CalcShape();

		public:
			CRenderNodeSprite();
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);

			inline void SetMatrix(const util::math::Matrix2D& mat) { m_matrix = mat; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetAnchor(const util::math::vec2& anchor) { m_anchor = anchor; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetTexture(SDL_Texture* const tex) { m_pTexture = tex; }
			inline void SetFlip(const SDL_RendererFlip& flip) { m_flip = flip; m_flag = NodeStateFlag::MOVE_DIRTY | NodeStateFlag::CULL_DIRTY; }
			inline void SetBlendMode(const SDL_BlendMode& blend) { m_blend_mode = blend; }
			
			inline const util::math::Matrix2D&		GetMatrix() const { return m_matrix; }
			inline const util::math::vec2&			GetAnchor() const { return m_anchor; }
			inline SDL_Texture* const				GetTexture() const { return m_pTexture; }
			inline const SDL_RendererFlip&			GetFlip() const { return m_flip; }
			inline const SDL_BlendMode&				GetBlendMode() const { return m_blend_mode; }
		};
	}
}