#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			class CRectPart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CRectPart);

				/*Data*/
			private:
				util::shape::AABB m_aabb;
			protected:

				/*Func*/
			public:
				CRectPart();
				virtual ~CRectPart();
				CRectPart& operator= (const CRectPart& that);
			
				virtual void OnMatrixChanged(void);

				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

			};
		}
	}
}