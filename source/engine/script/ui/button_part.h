#pragma once
#include "ui_part.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{

			class CButton : public CUiPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CButton);

				/*Data*/
			private:
			protected:
				bool m_bPressed;

				/*Func*/
			protected:
				virtual bool OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action);
				virtual bool OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action);
				virtual bool OnMouseMotionInternal(const input::mouse_events::MotionAction& action);

			public:
				CButton();
				virtual ~CButton();
				CButton& operator= (const CButton& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);
			};
		}
	}
}