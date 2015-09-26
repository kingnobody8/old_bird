#pragma once
#include "../part/part.h"
#include "../math/math.h"
using namespace Engine::Math;
#include "../object/object.h"

namespace Engine
{
	namespace Script
	{
		class TestPart : public Part::IPart
		{
		private:
			Time time;
			float dist;
			float half;
			bool stop;
		public:
			TestPart(void)
			{
				this->m_szType = "TestPart";
				time = 0.0f;
				dist = RandomFloat(50, 100);
				half = RandomFloat(1.0f, 10.0f);
				stop = false;
			}
			void Update(Time delta)
			{
				if (stop)
					return;
				this->time += delta;
				/*Type3<float> pos = this->m_pOwner->GetLocalPos();
				pos.x = dist * sin(time * half);
				pos.y = dist * cos(time * half);
				this->m_pOwner->SetLocalPos(pos);*/

				auto parts = this->m_pOwner->GetParts();
				for (ulong i = 0; i < parts.size(); ++i)
				{
					if (parts[i]->GetType() == "CImg")
					{
						CImg* pImg = dynamic_cast<CImg*>(parts[i]);
						//pImg->SetAngle(time * 0.1f);
					}
				}
			}

			/*void onSpace(Event::IEvent* pEvent)
			{
				this->stop = !stop;
			}*/
		};
		DeclarePart(TestPart, PART_TESTPART)
	}
}