#pragma once
#include "util_time.h"
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
				SDL_Keycode m_code;
				bool m_repeat;
				KeyAction(const SDL_Event& event, const SDL_Keycode& code, const bool repeat)
					: IEvent(event), m_code(code), m_repeat(repeat)
				{
				}
			};

			static util::event::Publisher<KeyAction> s_InputKeyDown;
			static util::event::Publisher<KeyAction> s_InputKeyUp;
		};

	}
}