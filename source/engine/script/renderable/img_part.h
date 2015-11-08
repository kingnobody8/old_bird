#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			//NOTE: due to the limitations of SDL render rects, the final rotation is ignored and the rect is rendered as an AABB

			class CImgPart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CImgPart);

				/*Data*/
			private:
				std::string m_path;

			protected:

				/*Func*/
			public:
				CImgPart();
				virtual ~CImgPart();
				CImgPart& operator= (const CImgPart& that);
			
				virtual void Init();
				virtual void LoadJson(const util::JSON& json);
				virtual void LoadImgPath(const std::string& szPath);

				virtual void OnMatrixChanged(void);
				virtual void OnZedChanged(void);
			};
		}
	}
}