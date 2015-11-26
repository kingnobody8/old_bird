#include "fixture_part.h"
#include "script/script.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			DEFINE_PART_TYPE_INFO(CFixturePart);

			CFixturePart::CFixturePart()
			{
				m_priority = script::FIXTURE_PART;
			}
			VIRTUAL CFixturePart::~CFixturePart()
			{
			}
			CFixturePart& CFixturePart::operator = (const CFixturePart& that)
			{
				return *this;
			}

			VIRTUAL void CFixturePart::Init()
			{
			}

			VIRTUAL void CFixturePart::LoadJson(const util::JSON& json)
			{
				IBox2DPart::LoadJson(json);
			}
		}
	}
}