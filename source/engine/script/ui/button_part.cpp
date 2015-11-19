#include "button_part.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			DEFINE_PART_TYPE_INFO(CButton);

			CButton::CButton()
				: m_bPressed(false)
			{
			}

			CButton::~CButton()
			{
				DisableTouchImmediate();
			}

			VIRTUAL void CButton::Init()
			{
			}

			VIRTUAL void CButton::Exit()
			{
			}

			VIRTUAL void CButton::LoadJson(const util::JSON& json)
			{
			}

			VIRTUAL bool CButton::OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action)
			{
				if (CUiPart::OnMouseButtonDownInternal(action)) //if no intersection
					return true;



			}
		
			VIRTUAL bool CButton::OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action)
			{
			}
			
			VIRTUAL bool CButton::OnMouseMotionInternal(const input::mouse_events::MotionAction& action)
			{
			}

		
		}
	}
}