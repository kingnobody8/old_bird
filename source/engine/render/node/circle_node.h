#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeCircle : public IRenderNode
		{
		private:
			util::math::vec2 m_center;
			float m_radius = 0.0f;
			bool m_fill = true;

		protected:
			virtual const b2PolygonShape& CalcShape();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);

			void SetCenter(const util::math::vec2& center);
			void SetRadius(const float& radius);
			inline void SetFill(const bool fill) { m_fill = fill; }

			inline const util::math::vec2& GetCenter() const { return m_center; }
			inline const float& GetRadius() const { return m_radius; }
			inline const bool GetFill() const { return m_fill; }
		};
	}
}