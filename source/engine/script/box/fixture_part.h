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
				enum BODY_TYPE { BT_INVALID = -1, BT_STATIC, BT_DYNAMIC, BT_COUNT };

				/*Data*/
			private:
			protected:
				bool m_bSettingMatrix;
				BODY_TYPE m_bodyType;
				b2Body* m_pBody;
				vec2 c1;
				vec2 c2;

				/*Func*/
			public:
				CFixturePart();
				virtual ~CFixturePart();
				CFixturePart& operator= (const CFixturePart& that);

				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

				virtual void Update(const util::Time& dt);

				virtual void OnMatrixChanged();
			};
		}
	}
}