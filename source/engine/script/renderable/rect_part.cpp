#include "rect_part.h"
#include "../script_priorities.h"
#include "render/render_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CRectPart);

			CRectPart::CRectPart()
			{
				m_priority = EPriorities::RECT_PART;
				m_pNode = new render::CRenderNodeRect();
			}
			VIRTUAL CRectPart::~CRectPart()
			{
				delete m_pNode;
			}
			CRectPart& CRectPart::operator = (const CRectPart& that)
			{
				IRenderPart::operator=(that);
				m_aabb = that.m_aabb;
				return *this;
			}

			VIRTUAL void CRectPart::OnMatrixChanged(void)
			{
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				const util::math::vec2 offset = wmat.GetPosition() - m_aabb.CalcCenter();
				m_aabb.m_min += offset;
				m_aabb.m_max += offset;
				static_cast<render::CRenderNodeRect*>(m_pNode)->SetAABB(m_aabb);
			}

			VIRTUAL void CRectPart::OnZedChanged(void)
			{
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			VIRTUAL void CRectPart::Init()
			{
				IRenderPart::Init();
				
				//Setup the positions
				OnMatrixChanged();
				OnZedChanged();

				//static_cast<render::CRenderNodeRect*>(m_pNode)->SetAABB(m_aabb);
			}

			VIRTUAL void CRectPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				m_aabb = ((const util::JSON&)(json["aabb"])).GetAabb();
			}
		}
	}
}