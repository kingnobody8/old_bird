#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			//NOTE: due to the limitations of SDL render rects, the final rotation is ignored and the rect is rendered as an AABB

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
			
				virtual void LoadJson(const util::JSON& json);

				virtual void OnMatrixChanged(void);
				virtual void OnZedChanged(void);
			};
		}
	}
}