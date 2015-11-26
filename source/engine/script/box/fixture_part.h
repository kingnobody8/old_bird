#pragma once
#include "box2d_part.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			class CFixturePart : public IBox2DPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CFixturePart);

				/*Data*/
			private:
			protected:
				bool m_bSettingMatrix;

				/*Func*/
			public:
				CFixturePart();
				virtual ~CFixturePart();
				CFixturePart& operator= (const CFixturePart& that);

				virtual void Init();
				virtual void LoadJson(const util::JSON& json);
			};
		}
	}
}