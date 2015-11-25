#include "button_part.h"
#include "component/object.h"
#include "../script.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			DEFINE_PART_TYPE_INFO(CButtonPart);

			CButtonPart::CButtonPart()
				: m_bPressed(false)
			{
				m_priority = BUTTON_PART;
			}

			CButtonPart::~CButtonPart()
			{
			}

			VIRTUAL void CButtonPart::Init()
			{
				CUiPart::Init();
			}

			VIRTUAL void CButtonPart::Exit()
			{
				CUiPart::Exit();
			}

			VIRTUAL void CButtonPart::LoadJson(const util::JSON& json)
			{
				CUiPart::LoadJson(json);
			}

			VIRTUAL bool CButtonPart::OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action, const util::math::vec2& wpos)
			{
				if (!CUiPart::OnMouseButtonDownInternal(action, wpos)) //if no intersection
					return true;

				return true;
			}
		
			VIRTUAL bool CButtonPart::OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action, const util::math::vec2& wpos)
			{
				if (!CUiPart::OnMouseButtonUpInternal(action, wpos)) //if no intersection
					return true;

				return true;
			}
			
			VIRTUAL bool CButtonPart::OnMouseMotionInternal(const input::mouse_events::MotionAction& action, const util::math::vec2& wpos)
			{
				if (!CUiPart::OnMouseMotionInternal(action, wpos)) //if no intersection
					return true;

				return true;
			}

		
		}
	}
}