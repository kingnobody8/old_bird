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
				const bool intersection = CUiPart::OnMouseButtonDownInternal(action, wpos);

				if (m_bPressed) //if we are pressed, then return that we handled it (may happen when screen loses focus while holding)
					return true;
				else if (!intersection) //else no intersection, so return unhandled
					return false;

				m_bPressed = true;
				return true;
			}
		
			VIRTUAL bool CButtonPart::OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action, const util::math::vec2& wpos)
			{
				if (!m_bPressed)
					return false;

				const bool intersection = CUiPart::OnMouseButtonUpInternal(action, wpos);

				if (m_bPressed)
				{
					m_bPressed = false;
					if (intersection)
					{
						__todo() //click callback here
						int x = 0;
						x++;
					}
					return true;
				}
				return false;
			}
			
			VIRTUAL bool CButtonPart::OnMouseMotionInternal(const input::mouse_events::MotionAction& action, const util::math::vec2& wpos)
			{
				if (!m_bPressed)
					return false;

				const bool intersection = CUiPart::OnMouseMotionInternal(action, wpos);
					
				if (m_bPressed)
				{
					if (!intersection)
					{
						m_bPressed = false;
					}
					//else do nothing
					return true;
				}
				return false;
			}
		}
	}
}