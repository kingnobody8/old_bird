#pragma once
#include "ui_part.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{

			class CButtonPart : public CUiPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CButtonPart);

				/*Data*/
			private:
			protected:
				bool m_bPressed;

				/*Func*/
			protected:
				virtual bool OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action, const util::math::vec2& wpos);
				virtual bool OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action, const util::math::vec2& wpos);
				virtual bool OnMouseMotionInternal(const input::mouse_events::MotionAction& action, const util::math::vec2& wpos);

			public:
				CButtonPart();
				virtual ~CButtonPart();
				CButtonPart& operator= (const CButtonPart& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);
			};
		}
	}
}