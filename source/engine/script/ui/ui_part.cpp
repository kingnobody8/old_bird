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
				if (lhs->GetLayer() == rhs->GetLayer())
					return lhs->GetZed() < rhs->GetZed();
				return lhs->GetLayer()->GetSortRank() > rhs->GetLayer()->GetSortRank();
			}

			static const u16 INVALID_TOUCH_INDEX = 0xFFFF;

			STATIC std::list<CUiPart::UiLayer> CUiPart::s_uiLayers;
			STATIC CUiPart::UiPartList CUiPart::s_pendingUiParts;

			STATIC const CUiPart::UiPartList CUiPart::GetUiParts()
			{
				UiPartList ret;
				for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
				{
					ret.insert(ret.end(), (*iter).m_uiParts.begin(), (*iter).m_uiParts.end());
				}
				return ret;
			}

			STATIC void CUiPart::CleanTouchParts(void)
			{
				bool appended = false;
				for (UiPartIter iter = s_pendingUiParts.begin(); iter != s_pendingUiParts.end(); ++iter)
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
				s_pendingUiParts.clear();

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

			VIRTUAL bool CUiPart::OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action)
			{
				return true;
			}

			VIRTUAL bool CUiPart::OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action)
			{
				return true;
			}

			void CUiPart::Register()
			{
				if (m_bTouchEnabled)
					return;
				m_bTouchEnabled = true;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingUiParts.push_back(this);
			}

			void CUiPart::Unregister()
			{
				if (!m_bTouchEnabled)
					return;
				m_bTouchEnabled = false;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingUiParts.push_back(this);
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