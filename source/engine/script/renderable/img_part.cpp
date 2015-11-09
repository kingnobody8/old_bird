#include "img_part.h"
#include "../script.h"
#include "render/render_node.h"
#include "component/object.h"
#include "asset/loader.h"
#include "asset/resource_path.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CImgPart);

			CImgPart::CImgPart()
			{
				m_priority = EPriorities::IMG_PART;
				m_pNode = new render::CRenderNodeSprite();
			}
			VIRTUAL CImgPart::~CImgPart()
			{
				delete m_pNode;
			}
			CImgPart& CImgPart::operator = (const CImgPart& that)
			{
				IRenderPart::operator=(that);
				m_path = that.m_path;
				return *this;
			}

			VIRTUAL void CImgPart::Init()
			{
				IRenderPart::Init();

				LoadImgPath(m_path);
				SetAnchor(m_anchor);
				SetBlendMode(m_blend_mode);
				SetFlip(m_flip);
			}

			VIRTUAL void CImgPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				
				auto asdf = json.Read();

				assert(json.HasMember("path"));
				assert(json.HasMember("blend_mode"));
				assert(json.HasMember("flip"));
				assert(json.HasMember("anchor"));

				m_path = json["path"].GetString();
				m_blend_mode = (SDL_BlendMode)json["blend_mode"].GetInt();
				m_flip = (SDL_RendererFlip)json["flip"].GetInt();
				m_anchor = ((const util::JSON&)(json["anchor"])).GetVec2();

				assert(!m_path.empty());
			}

			VIRTUAL void CImgPart::LoadImgPath(const std::string& szPath)
			{
				m_path = szPath;
				SDL_Texture* tex = asset::FileLoaderSdlTexture(getResourcePath() + szPath);
				static_cast<render::CRenderNodeSprite*>(m_pNode)->SetTexture(tex);
			}

			VIRTUAL void CImgPart::OnMatrixChanged(void)
			{
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				static_cast<render::CRenderNodeSprite*>(m_pNode)->SetMatrix(wmat);
			}

			VIRTUAL void CImgPart::OnZedChanged(void)
			{
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			void CImgPart::SetAnchor(const util::math::vec2& anchor)
			{
				m_anchor = anchor;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetAnchor(anchor);
			}

			void CImgPart::SetFlip(const SDL_RendererFlip& flip)
			{
				m_flip = flip;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetFlip(flip);
			}
			
			void CImgPart::SetBlendMode(const SDL_BlendMode& blend_mode)
			{
				m_blend_mode = blend_mode;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetBlendMode(blend_mode);
			}
		}
	}
}