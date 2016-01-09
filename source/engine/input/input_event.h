#pragma once
#include "util_time.h"
#include "math/math.h"
#include "event/publisher.h"
#include "event/publisher.inl"
#include "SDL.h"

namespace engine
{
	namespace input
	{
		struct IEvent
		{
			SDL_Event m_event;
			IEvent(const SDL_Event& event)
				: m_event(event)
			{
			}
		};

		struct app_events
		{
			static util::event::VoidPublisher s_KillAppEvent;
		};

		struct key_events
		{
			struct KeyAction : public IEvent
			{
				SDL_Keycode		m_code;
				bool			m_repeat;

				KeyAction(const SDL_Event& event, const SDL_Keycode& code, const bool repeat)
					: IEvent(event), m_code(code), m_repeat(repeat)
				{
				}
			};

			static util::event::Publisher<KeyAction> s_InputKeyDown;
			static util::event::Publisher<KeyAction> s_InputKeyUp;
		};

		struct mouse_events
		{
			struct MotionAction : public IEvent
			{
				vec2	m_pixel;
				vec2	m_delta;
				__todo() // we may eventually want velocity in here delta_pixels / time_since last motion

					MotionAction(const SDL_Event& event, const vec2& pixel, const vec2& delta)
					: IEvent(event), m_pixel(pixel), m_delta(delta)
				{
				}
			};

			struct ButtonAction : public IEvent
			{
				vec2	m_pixel;
				uchar	m_button; //this is SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, or SDL_BUTTON_RIGHT
				uchar	m_clicks;

				ButtonAction(const SDL_Event& event, const vec2& pixel, const uchar& button, const uchar& clicks)
					: IEvent(event), m_pixel(pixel), m_button(button), m_clicks(clicks)
				{
				}
			};

			struct WheelAction : public IEvent
			{
				vec2	m_scroll;

				WheelAction(const SDL_Event& event, const vec2 scroll)
					: IEvent(event), m_scroll(scroll)
				{
				}
			};

			static util::event::Publisher<MotionAction> s_InputMouseMotion;
			static util::event::Publisher<ButtonAction> s_InputMouseButtonDown;
			static util::event::Publisher<ButtonAction> s_InputMouseButtonUp;
			static util::event::Publisher<WheelAction>	s_InputMouseScrollWheel;
		};
	}
}