#pragma once
#include "render_node.h"

namespace engine
{
	namespace render
	{
		class CRenderNodeCircle : public IRenderNode
		{
		private:
			b2CircleShape m_circle;
			bool m_fill = true;

		protected:
			virtual void CalcAabbInternal();

		public:
			virtual void operator () (SDL_Renderer* pRen, const util::math::Matrix2D& inv_cam);

			void SetCircle(const b2CircleShape& circle);
			inline void SetFill(const bool fill) { m_fill = fill; }

			inline const b2CircleShape& GetCircle() const { return m_circle; }
			inline const bool GetFill() const { return m_fill; }
		};
	}
}