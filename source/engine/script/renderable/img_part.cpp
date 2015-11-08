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
			}

			VIRTUAL void CImgPart::LoadJson(const util::JSON& json)
			{
				auto asdf = json.Read();

				IRenderPart::LoadJson(json);
				m_path = json["path"].GetString();
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
		}
	}
}