#pragma once
#include "../part/part.h"

namespace Engine
{
	namespace Script
	{
		class CPrefab : public Part::IPart
		{
		private:
		public:
			CPrefab(void);
			virtual void LoadJson(const rapidjson::Value& json);

		};
		DeclarePart(CPrefab, PART_PREFAB)
	}
}