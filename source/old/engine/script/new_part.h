#pragma once
#include "../part/part.h"

namespace Script
{
	class INewPart : public Part::IPart
	{
		float y_off;
		float time;

	public:
		INewPart(void)
		{
			this->m_szType = "INewPart";
			time = 0.0f;
			y_off = 0.0f;
		}

		virtual void Update(Time fDelta)
		{
			time += fDelta;
			Type3<float> pos = this->m_pOwner->GetLocalPos();
			pos.y = 64 * cos(time * 10);
			this->m_pOwner->SetLocalPos(pos);
		}
	};
	DeclarePart(INewPart, PART_INEWPART)

}