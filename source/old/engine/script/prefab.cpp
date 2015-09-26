#include "prefab.h"

namespace Engine
{
	namespace Script
	{
		CPrefab::CPrefab(void)
		{
			this->m_szType = "CPrefab";
		}

		/*virtual*/ void CPrefab::LoadJson(const rapidjson::Value& json)
		{

		}

	}
}