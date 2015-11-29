#pragma once
#include "Collision/Shapes/b2PolygonShape.h"
#include "bit_flag.h"
#include "sdl/include/SDL.h"
#include "math/matrix_2d.h"
#include "shape/aabb.h"
#include <string>

namespace engine
{
	namespace render
	{
		//Forward Declare
		class CRenderLayer;

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
			util::shape::AABB	m_scissor;
			bool				m_use_scissor;
			float				m_zed;

		protected:
			void ScissorOperation(SDL_Renderer* pRen, const util::math::vec2& origin);
			virtual const b2PolygonShape& CalcShape() = 0;

		public:
			IRenderNode(void);
			virtual ~IRenderNode() = 0;
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam) = 0;
			
			const util::shape::AABB CalcAABB();
			const bool CheckInView(const b2PolygonShape& view);

			inline void SetColor(const SDL_Color& clr) { m_clr = clr; }
			inline void SetZed(const float& zed) { m_zed = zed; }

			inline const SDL_Color& GetColor() const { return m_clr; }
			inline const float& GetZed() const { return m_zed; }
			inline CRenderLayer* GetLayer() const { return m_pLayer; }

			void SetScissorRect(const util::shape::AABB& scissor);
			inline void ClearScissorRect() { m_use_scissor = false; }
			inline const util::shape::AABB& GetScissorRect() const { return (m_use_scissor) ? m_scissor : util::shape::AABB::INVALID_AABB; }
			inline const bool IsScissorActive() const { return m_use_scissor; }

			void Register(const std::string& layer);
			void Unregister(void);
		};
	}
}