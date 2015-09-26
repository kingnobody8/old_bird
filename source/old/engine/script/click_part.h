#pragma once
#include "engine/component/part.h"
#include "engine/math/rect.h"
#include "engine/util/util.h"
using namespace Engine::Util;
#include "engine/input/input_event.h"

namespace Engine
{
	namespace Script
	{
		class CClickPart : public Part::IPart
		{
		public:
			DECLARE_PART_TYPE_INFO(CClickPart);

			/*Data*/
		protected:
			bool m_mdown;
			bool m_ldown;
			bool m_rdown;
			Input::CRequestResolution m_res;
			vec2 mouse_world_pos;
			int type;

			/*Func*/
		protected:
			void SetPosition(vec2 pos);
			void DoPush(vec2 pos);
			void DoPull(vec2 pos);
			void DoSwipe(vec2 vel);
		public:
			CClickPart(void);
			virtual	~CClickPart(void);

			virtual void Init(void);
			virtual void Exit(void);
			virtual void Update(const Util::Time& dt);

			PROTO_EVENT_CALLBACK(OnResChanged, Engine::Input::CResolutionChange);
			PROTO_EVENT_CALLBACK(OnMouseDown, Engine::Input::CMouseButtonDownEvent);
			PROTO_EVENT_CALLBACK(OnMouseMove, Engine::Input::CMouseMotionEvent);
			PROTO_EVENT_CALLBACK(OnMouseUp, Engine::Input::CMouseButtonUpEvent);
			PROTO_EVENT_CALLBACK(OnKeyUp, Engine::Input::CKeyUpEvent);
		};
	}
}