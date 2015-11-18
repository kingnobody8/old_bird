#include "ui_part.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			DEFINE_PART_TYPE_INFO(CUiPart);

			bool SortFunc(CUiPart* lhs, CUiPart* rhs)
			{
				__todo() // this is where we will sort the ui parts by layer then zed, but we need also need to make it so that render layer no longer uses a desc, since all the information about it should reside inside itself
				render::CRenderLayer* lay;
			}

			static const u16 INVALID_TOUCH_INDEX = 0xFFFF;

			STATIC CUiPart::UiPartList CUiPart::s_touchParts;
			STATIC CUiPart::UiPartList CUiPart::s_pendingTouchParts;

			STATIC void CUiPart::CleanTouchParts(void)
			{
				bool appended = false;
				for (UiPartIter iter = s_pendingTouchParts.begin(); iter != s_pendingTouchParts.end(); ++iter)
				{
					CUiPart* part = (*iter);
					part->m_bPendingTouch = false;
					if (part->m_bTouchEnabled) //Enable
					{
						// Guard against enabling multiple times
						if (part->m_usTouchIndex == INVALID_TOUCH_INDEX)
						{
							part->m_usTouchIndex = (u16)s_touchParts.size();
							s_touchParts.push_back(part);
							appended = true;
						}
					}
					else //Disable
					{
						if (part->m_usTouchIndex != INVALID_TOUCH_INDEX)
						{
							if (!s_touchParts.empty())
							{
								UiPartIter ours = s_touchParts.begin();
								std::advance(ours, part->m_usTouchIndex);
								UiPartIter back = s_touchParts.end();
								std::advance(back, -1);
								std::swap(ours, back);
								s_touchParts.pop_back();
							}
							//Invalidate the index
							part->m_usTouchIndex = INVALID_TOUCH_INDEX;
						}
					}
				}
				s_pendingTouchParts.clear();

				if (appended)
					s_touchParts.sort();
			}

			STATIC void CUiPart::OnMouseButtonDown(const input::mouse_events::ButtonAction& action)
			{
				CleanTouchParts();
				for (UiPartIter part_iter = s_touchParts.begin(); part_iter != s_touchParts.end(); ++part_iter)
				{
					if ((*part_iter)->m_bTouchEnabled)
					{
						if (!(*part_iter)->OnMouseButtonDownInternal(action))
							return;
					}
				}
			}
			
			STATIC void CUiPart::OnMouseButtonUp(const input::mouse_events::ButtonAction& action)
			{
				CleanTouchParts();
				for (UiPartIter part_iter = s_touchParts.begin(); part_iter != s_touchParts.end(); ++part_iter)
				{
					if ((*part_iter)->m_bTouchEnabled)
					{
						if (!(*part_iter)->OnMouseButtonUpInternal(action))
							return;
					}
				}
			}

			STATIC void CUiPart::OnMouseMotion(const input::mouse_events::MotionAction& action)
			{
				CleanTouchParts();
				for (UiPartIter part_iter = s_touchParts.begin(); part_iter != s_touchParts.end(); ++part_iter)
				{
					if ((*part_iter)->m_bTouchEnabled)
					{
						if (!(*part_iter)->OnMouseMotionInternal(action))
							return;
					}
				}
			}

			CUiPart::CUiPart()
				: m_bTouchEnabled(false)
				, m_bPendingTouch(false)
				, m_usTouchIndex(INVALID_TOUCH_INDEX)
			{
			}

			CUiPart::~CUiPart()
			{
				DisableTouchImmediate();
			}

			void CUiPart::CalculateIntersectionRect()
			{
				m_aabb = m_pOwner->CalcAabb();
			}

			VIRTUAL void CUiPart::Init()
			{
			}

			VIRTUAL void CUiPart::Exit()
			{
			}

			VIRTUAL void CUiPart::LoadJson(const util::JSON& json)
			{
			}

			VIRTUAL void CUiPart::OnMatrixChanged(void)
			{
				CalculateIntersectionRect();
			}

			VIRTUAL void CUiPart::OnChildMatricChanged(component::CObject* child)
			{
				CalculateIntersectionRect();
			}

			void CUiPart::Register()
			{
				if (m_bTouchEnabled)
					return;
				m_bTouchEnabled = true;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingTouchParts.push_back(this);
			}

			void CUiPart::Unregister()
			{
				if (!m_bTouchEnabled)
					return;
				m_bTouchEnabled = false;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingTouchParts.push_back(this);
			}

			void CUiPart::DisableTouchImmediate(void)
			{
				if (this->m_usTouchIndex != INVALID_TOUCH_INDEX)
				{
					if (!s_touchParts.empty())
					{
						UiPartIter ours = s_touchParts.begin();
						std::advance(ours, this->m_usTouchIndex);
						UiPartIter back = s_touchParts.end();
						std::advance(back, -1);
						std::swap(ours, back);
						s_touchParts.pop_back();
					}
					//Invalidate the index
					this->m_usTouchIndex = INVALID_TOUCH_INDEX;
				}
			}
		}
	}
}