#include "touch_part.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		static const u16 INVALID_TOUCH_INDEX = 0xFFFF;


		STATIC CTouchPart::TouchPartList CTouchPart::s_touchParts;
		STATIC CTouchPart::TouchPartList CTouchPart::s_pendingTouchParts;

		STATIC void CTouchPart::CleanTouchParts(void)
		{
			for (TouchPartIter iter = s_pendingTouchParts.begin(); iter != s_pendingTouchParts.end(); ++iter)
			{
				CTouchPart* part = (*iter);
				part->m_bPendingTouch = false;
				if (part->m_bTouchEnabled) //Enable
				{
					// Guard against enabling multiple times
					if (part->m_usTouchIndex == INVALID_TOUCH_INDEX)
					{
						part->m_usTouchIndex = (u16)s_touchParts.size();
						s_touchParts.push_back(part);
					}
				}
				else //Disable
				{
					if (part->m_usTouchIndex != INVALID_TOUCH_INDEX)
					{
						if (!s_touchParts.empty())
						{
							TouchPartIter ours = s_touchParts.begin();
							std::advance(ours, part->m_usTouchIndex);
							TouchPartIter back = s_touchParts.end();
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
		}


		CTouchPart::CTouchPart()
			: m_bTouchEnabled(false)
			, m_bPendingTouch(false)
			, m_usTouchIndex(INVALID_TOUCH_INDEX)
		{
		}

		CTouchPart::~CTouchPart()
		{
			DisableTouchImmediate();
		}

		void CTouchPart::CalculateIntersectionRect()
		{
			m_aabb = m_pOwner->CalcAabb();
		}

		VIRTUAL void CTouchPart::Init()
		{
		}

		VIRTUAL void CTouchPart::Exit()
		{
		}

		VIRTUAL void CTouchPart::LoadJson(const util::JSON& json)
		{
		}

		VIRTUAL void CTouchPart::OnMatrixChanged(void)
		{
			CalculateIntersectionRect();
		}

		VIRTUAL void CTouchPart::OnChildMatricChanged(component::CObject* child)
		{
			CalculateIntersectionRect();
		}

		void CTouchPart::Register()
		{
			if (m_bTouchEnabled)
				return;
			m_bTouchEnabled = true;

			if (m_bPendingTouch)
				return;
			m_bPendingTouch = true;

			s_pendingTouchParts.push_back(this);
		}

		void CTouchPart::Unregister()
		{
			if (!m_bTouchEnabled)
				return;
			m_bTouchEnabled = false;

			if (m_bPendingTouch)
				return;
			m_bPendingTouch = true;

			s_pendingTouchParts.push_back(this);
		}

		void CTouchPart::DisableTouchImmediate(void)
		{
			if (this->m_usTouchIndex != INVALID_TOUCH_INDEX)
			{
				if (!s_touchParts.empty())
				{
					TouchPartIter ours = s_touchParts.begin();
					std::advance(ours, this->m_usTouchIndex);
					TouchPartIter back = s_touchParts.end();
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