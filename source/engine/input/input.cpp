#include "input.h"
#include <assert.h>

namespace engine
{
	namespace input
	{
		static SDL_Renderer* s_sdlRen = null;

		STATIC util::event::Publisher<key_events::KeyAction> key_events::s_InputKeyDown;
		STATIC util::event::Publisher<key_events::KeyAction> key_events::s_InputKeyUp;

		STATIC util::event::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonDown;
		STATIC util::event::Publisher<mouse_events::ButtonAction> mouse_events::s_InputMouseButtonUp;

		const util::math::Type2<slong> ConvertPixelToCartesian(int x, int y)
		{
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(s_sdlRen, &logical_size.w, &logical_size.h);

			util::math::Type2<slong> ret;
			ret.x = (slong)x;
			ret.y = (slong)(logical_size.h - y);
			return ret;
		}


		void Setup(SDL_Renderer* pRen)
		{
			assert(pRen);
			s_sdlRen = pRen;
		}

		bool PollSdl(const util::Time& dt)
		{
			SDL_Event tEvent;
			while (SDL_PollEvent(&tEvent))
			{
				switch (tEvent.type)
				{
					//QUIT event
				case SDL_QUIT:
					return false; //NOTE if we get a quit event, then we want to exit ASAP!
					//MOBILE event
				case SDL_APP_TERMINATING:
				case SDL_APP_LOWMEMORY:
				case SDL_APP_WILLENTERBACKGROUND:
				case SDL_APP_DIDENTERBACKGROUND:
				case SDL_APP_WILLENTERFOREGROUND:
				case SDL_APP_DIDENTERFOREGROUND:
					break;
					//SYSTEM event
				case SDL_WINDOWEVENT:
					break;
				case SDL_SYSWMEVENT:	/*Disabled by default, system specific events, to enable call SDL_EventState*/
					break;
					//KEYBOARD event
				case SDL_KEYDOWN:
					key_events::s_InputKeyDown.Publish(key_events::KeyAction(tEvent, tEvent.key.keysym.sym, tEvent.key.state != 0));
					break;
				case SDL_KEYUP:
					key_events::s_InputKeyUp.Publish(key_events::KeyAction(tEvent, tEvent.key.keysym.sym, tEvent.key.state != 0));
					break;
					//TEXT event
				case SDL_TEXTEDITING:
				case SDL_TEXTINPUT:
					//MOUSE event
					break;
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouse_events::s_InputMouseButtonDown.Publish(mouse_events::ButtonAction(tEvent, ConvertPixelToCartesian(tEvent.motion.x, tEvent.motion.y), tEvent.button.button, tEvent.button.clicks));
					break;
				case SDL_MOUSEBUTTONUP:
					mouse_events::s_InputMouseButtonUp.Publish(mouse_events::ButtonAction(tEvent, ConvertPixelToCartesian(tEvent.motion.x, tEvent.motion.y), tEvent.button.button, tEvent.button.clicks));
					break;
				case SDL_MOUSEWHEEL:
					break;
					//JOY event
				case SDL_JOYAXISMOTION:
				case SDL_JOYBALLMOTION:
				case SDL_JOYHATMOTION:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				case SDL_JOYDEVICEADDED:
				case SDL_JOYDEVICEREMOVED:
					break;
					//CONTROLLER event
				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
				case SDL_CONTROLLERDEVICEADDED:
				case SDL_CONTROLLERDEVICEREMOVED:
				case SDL_CONTROLLERDEVICEREMAPPED:
					break;
					//TOUCH event
				case SDL_FINGERDOWN:
				case SDL_FINGERUP:
				case SDL_FINGERMOTION:
					break;
					//GESTURE event
				case SDL_DOLLARGESTURE:
				case SDL_DOLLARRECORD:
				case SDL_MULTIGESTURE:
					break;
					//CLIPBOARD event
				case SDL_CLIPBOARDUPDATE:
					break;
					//DROP event
				case SDL_DROPFILE:
					break;
					//USER event
				case SDL_USEREVENT:
				case SDL_LASTEVENT: /*Don't use*/
					break;
				default:
					break;
				}
			}
			return true;
		}

	}
}