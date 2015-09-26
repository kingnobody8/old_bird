#pragma once
#include "engine/util/util.h"
using namespace Engine::Util;
#include "engine/util/color.h"
#include "engine/script/render_part.h"
#include "engine/render/render_node.h"

//Forward Declare
namespace Asset { class CAsset; }

namespace Engine
{
	namespace Script
	{
		class CImg : public IRenderPart
		{
		public:
			DECLARE_PART_TYPE_INFO(CImg);

			/*Data*/
		private:
		protected:
			slong				m_aImg;
			Type2<float>		m_tAnchor;
			CLR::TColor			m_tColor;
			SDL_BlendMode		m_eBlendMode;
			SDL_RendererFlip	m_eRenderFlip;
			Render::CRenderNodeSprite	m_cSprite;

			/*Func*/
		public:
			CImg(void);
			virtual ~CImg(void);
			CImg& operator= (const CImg& that);

			virtual void Copy(Part::IPart* const that);
			virtual void LoadJson(const Json::JSON& json);
			//TODO add a savejson func

			virtual void OnAttach(void);
			virtual void OnMatrixChanged(void);
			virtual void OnVisibilityChanged(const bool visible);

			//Gets
			inline Type2<float> GetAnchor(void) const { return this->m_tAnchor; }
			inline const CLR::TColor& GetColor(void) const { return this->m_tColor; }
			virtual Engine::Math::AABB GetAABB(void) const;
			const Type2<int>			GetSourceSize(void) const;
			//Sets
			inline void SetImg(const slong& aImg) { this->m_aImg = aImg; }
			inline void SetAnchor(const Type2<float>& tAnchor) { this->m_tAnchor = Type2<float>(Clamp(tAnchor.x, 0.0f, 1.0f), Clamp(tAnchor.y, 0.0f, 1.0f)); }
			virtual void SetOwner(Object::CObject* const pOwner);
			inline void SetColor(const CLR::TColor& clr) { this->m_tColor = clr; }
			void SetRBG(const float& r, const float& g, const float& b);
			void SetAlpha(const float& a);

			virtual Render::IRenderNode* GetRenderNode(void);
		};
	}
}
