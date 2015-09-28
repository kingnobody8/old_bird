#include "rect_part.h"
#include "../script_priorities.h"

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
			}
			VIRTUAL CRectPart::~CRectPart()
			{

			}
			CRectPart& CRectPart::operator = (const CRectPart& that)
			{
				m_aabb = that.m_aabb;
				return *this;
			}

			VIRTUAL void CRectPart::LoadJson(const util::JSON& json)
			{
				m_aabb = ((const util::JSON&)(json["aabb"])).GetAabb();
			}
		}
	}
}