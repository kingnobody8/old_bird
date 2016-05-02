#include "input_event.h"

namespace engine
{
	//app
	STATIC util::VoidPublisher app_events::s_KillAppEvent;

	//key
	STATIC util::Publisher<key_events::KeyAction> key_events::s_InputKeyDown;
	STATIC util::Publisher<key_events::KeyAction> key_events::s_InputKeyUp;

	//mouse
	STATIC util::Publisher<mouse_events::MotionAction> mouse_events::s_InputMouseMotion;
	STATIC util::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonDown;
	STATIC util::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonUp;
	STATIC util::Publisher<mouse_events::WheelAction> mouse_events::s_InputMouseScrollWheel;
	
	//finger
	STATIC util::Publisher<finger_events::TouchAction> finger_events::s_InputFingerDown;
	STATIC util::Publisher<finger_events::TouchAction> finger_events::s_InputFingerUp;
	STATIC util::Publisher<finger_events::MotionAction> finger_events::s_InputTouchMotion;
}