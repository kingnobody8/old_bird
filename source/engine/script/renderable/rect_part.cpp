#include "rect_part.h"
#include "../script_priorities.h"
#include "render/render_node.h"

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
				__todo() //maybe this should pass in our local and world matricies
			}

			VIRTUAL void CRectPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				m_aabb = ((const util::JSON&)(json["aabb"])).GetAabb();
				static_cast<render::CRenderNodeRect*>(m_pNode)->SetAABB(m_aabb);
			}
		}
	}
}