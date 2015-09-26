#pragma once
#include "engine/math/math.h"
#include "engine/platform/inc_sdl.h"
#include <list>
#include <map>

namespace Engine
{
	namespace Render
	{
		//Forward Declare
		class CRenderLayer;

		class IRenderNode
		{
		protected:
			CRenderLayer* m_pLayer;
			Math::Type3<float>	position;
			Math::Type2<float>	scale;
			float				rotation;

		public:
			IRenderNode(void);
			virtual void operator () (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam) = 0;

			void Register(const std::string& layer);
			void Unregister(void);
			void SetMatrix(const Math::CMatrix2D& mat);
			void SetZed(const float& zed);
			const Math::CMatrix2D GetMatrix(void) const;
			inline const float GetZed(void) const { return position.z; }
		};

		class CRenderNodeSprite : public IRenderNode
		{
		public:
			SDL_Texture*		texture;
			SDL_RendererFlip	flip;
			SDL_Color			clr;
			SDL_Rect			src_rect;
			SDL_BlendMode		blend_mode;
			Math::Type2<float>	anchor;
			float				realAlpha;

		public:
			virtual void operator () (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam);
		};

		//class CRenderNodeText : public IRenderNode
		//{
		//	//TODO
		//public:
		//	virtual void operator () (SDL_Renderer* pRen, CMatrix2D camera);
		//};

		class CRenderNodeLine : public IRenderNode
		{
		public:
			SDL_Color clr;
			Math::vec2 v1;
			Math::vec2 v2;

		public:
			virtual void operator () (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam);
		};

		class CRenderNodeRect : public IRenderNode
		{
		public: SDL_Color clr;
				SDL_Rect dstRect;
				bool fill;
		public:
			virtual void operator () (SDL_Renderer* pRen, const Math::CMatrix2D& inv_cam);
		};
	}
}