#pragma once
#include "util_time.h"
#include "math/type2.h"
#include "event/publisher.h"
#include "event/publisher.inl"
#include "SDL.h"

namespace engine
{
	namespace input
	{
		void Setup(SDL_Renderer* pRen);
		bool PollSdl(const util::Time& dt);

		struct IEvent
		{
			SDL_Event m_event;
			IEvent(const SDL_Event& event)
				: m_event(event)
			{
			}
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
			struct ButtonAction : public IEvent
			{
				util::math::Type2<slong>	m_pixel;
				uchar						m_button; //this is SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, or SDL_BUTTON_RIGHT
				uchar						m_clicks;

				ButtonAction(const SDL_Event& event, const util::math::Type2<slong>& pixel, const uchar& button, const uchar& clicks)
					: IEvent(event), m_pixel(pixel), m_button(button), m_clicks(clicks)
				{
				}
			};

			static util::event::Publisher<ButtonAction> s_InputMouseButtonDown;
			static util::event::Publisher<ButtonAction> s_InputMouseButtonUp;

		};

	}
}