#pragma once
#include "Collision/Shapes/b2PolygonShape.h"
#include "bit_flag.h"
#include "sdl/include/SDL.h"
#include "math/math.h"
#include "color.h"
#include <string>
#include "box2d/box2d.h"

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
			b2AABB				m_aabb;
			util::Flag08		m_flag;
			util::Color			m_clr;
			b2AABB				m_scissor;
			bool				m_use_scissor;
			float				m_zed;

		protected:
			void ScissorOperation(SDL_Renderer* pRen, const vec2& origin);
			virtual void CalcAabbInternal() = 0;

		public:
			IRenderNode(void);
			virtual ~IRenderNode() = 0;
			virtual void operator () (SDL_Renderer* pRen, const matrix& inv_cam) = 0;
			
			const b2AABB CalcAABB();
			const bool CheckInView(const b2AABB& view);

			inline void SetColor(const util::Color& clr) { m_clr = clr; }
			inline void SetZed(const float& zed) { m_zed = zed; }

			inline const util::Color& GetColor() const { return m_clr; }
			inline const float& GetZed() const { return m_zed; }
			inline CRenderLayer* GetLayer() const { return m_pLayer; }

			void SetScissorRect(const b2AABB& scissor);
			inline void ClearScissorRect() { m_use_scissor = false; }
			inline const b2AABB& GetScissorRect() const { return /*(m_use_scissor) ?*/ m_scissor /*: util::shape::AABB::INVALID_AABB*/; }
			inline const bool IsScissorActive() const { return m_use_scissor; }

			void Register(const std::string& layer);
			void Unregister(void);
		};
	}
}