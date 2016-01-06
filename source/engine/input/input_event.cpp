#include "input_event.h"

namespace engine
{
	namespace input
	{
		//app
		STATIC util::event::VoidPublisher app_events::s_KillAppEvent;

		//key
		STATIC util::event::Publisher<key_events::KeyAction> key_events::s_InputKeyDown;
		STATIC util::event::Publisher<key_events::KeyAction> key_events::s_InputKeyUp;

		//mouse
		STATIC util::event::Publisher<mouse_events::MotionAction> mouse_events::s_InputMouseMotion;
		STATIC util::event::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonDown;
		STATIC util::event::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonUp;
		STATIC util::event::Publisher<mouse_events::WheelAction> mouse_events::s_InputMouseScrollWheel;
	}
}