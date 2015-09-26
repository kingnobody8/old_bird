#pragma once
#include "engine/util/util.h"
#include "engine/math/math.h"
using namespace Engine::Util;
using namespace Engine::Math;
#include "engine/event/event.h"
#include "engine/platform/inc_sdl.h"
#include <map>

namespace Engine
{
	namespace Input
	{
		struct CResolutionChange : public Event::IEvent
		{
			Type2<int> old_res;
			Type2<int> new_res;
			vec2 scale;
		};

		struct CRequestResolution : public Event::IEvent
		{
			Type2<int> default_res;
			Type2<int> curr_res;
			vec2 scale;
		};

		struct CKeyActionEvent : public Event::IEvent
		{
			SDL_Keycode		code;
			bool			repeat;
		};
		struct CKeyDownEvent : public CKeyActionEvent{};
		struct CKeyUpEvent : public CKeyActionEvent{};

		struct CPointerActionEvent : public Event::IEvent
		{
			std::map<std::string, Type2<slong>> layer_pos;
			Type2<slong>	pixel;
			Type2<slong>	delta;
			Type2<float>	velocity;
		};
		struct CPointerMotionEvent : public CPointerActionEvent{};
		struct CMouseMotionEvent : public CPointerActionEvent{};
		struct CTouchMotionEvent : public CPointerActionEvent{};

		struct CPointerButtonEvent : public Event::IEvent
		{
			std::map<std::string, Type2<slong>> layer_pos;
			Type2<slong>	pixel;
			uchar			button;
			uchar			clicks;
		};
		struct CPointerButtonDownEvent : public CPointerButtonEvent{};
		struct CPointerButtonUpEvent : public CPointerButtonEvent{};
		struct CMouseButtonDownEvent : public CPointerButtonEvent{};
		struct CMouseButtonUpEvent : public CPointerButtonEvent{};
		struct CTouchDownEvent : public CPointerButtonEvent{};
		struct CTouchUpEvent : public CPointerButtonEvent{};

		struct CMouseWheelEvent : public Event::IEvent
		{
			Type2<slong>	scroll;
		};
	}
}
