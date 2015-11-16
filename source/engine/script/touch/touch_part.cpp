#include "touch_part.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		CTouchPart::CTouchPart()
		{

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

	}
}