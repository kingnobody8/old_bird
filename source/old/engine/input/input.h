#pragma once
#include "engine/platform/inc_sdl.h"
#include "engine/util/util.h"
#include "engine/math/math.h"
#include "engine/input/input_event.h"

using namespace Engine::Util;
using namespace Engine::Math;

//TODO (daniel) possible create a base class for systems used by the engine, to load only what we need (input, rendering, audio, object manager)
//they do things like init, update, and exit

namespace Engine
{
	namespace Input
	{
		class CInput
		{
			/*data*/
		private:
			SDL_Renderer*	m_pRen;
			Type2<int>		m_tDefaultRes;
			Type2<int>		m_tCurrRes;
			vec2			m_tScreenScale;
			Time			m_fMouseElapsedTime;
			bool			m_bQuit;

			/*func*/
		private:
			void PollSDL(const Time& dt);
			void HandleWindowEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleKeyEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleMouseEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleTouchEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleTextEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleControllerEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleJoyEvent(const SDL_Event& tEvent, const Time& dt);
			void HandleAppEvent(const SDL_Event& tEvent, const Time& dt);

			void OnResolutionChange();
			PROTO_EVENT_CALLBACK(OnRequestResolution, CRequestResolution);
			std::map < std::string, Type2<slong>> FillOutLayerPos(Type2<slong> pixel);

		public:
			CInput();
			void Init(SDL_Renderer* const pRen);
			bool Update(const Time& dt);
		};
	}
}