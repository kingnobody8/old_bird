#include "engine/input/input.h"
#include "engine/input/input_event.h"
#include "engine/engine.h"
#include "engine/render/renderer.h"


namespace Engine
{
	namespace Input
	{
		CInput::CInput()
			: m_pRen(null)
			, m_fMouseElapsedTime(0.0f)
			, m_bQuit(true)
		{
			//TODO this shouldn't be hardcoded
			this->m_tDefaultRes = this->m_tCurrRes = Type2<int>(1080, 1920);
		}

		void CInput::Init(SDL_Renderer* const pRen)
		{
			assert(pRen);
			this->m_pRen = pRen;
			this->m_tDefaultRes = Type2<int>((int)Engine::default_dims.x, (int)Engine::default_dims.y);
			this->OnResolutionChange();
			m_bQuit = false;

			CRequestResolution().Subscribe(this, BIND1(this, &CInput::OnRequestResolution)); //TODO we need to add an exit func for input, to unsubscribe
		}

		bool CInput::Update(const Time& dt)
		{
			this->m_fMouseElapsedTime += dt;
			this->PollSDL(dt);
			return this->m_bQuit;
		}

		void CInput::PollSDL(const Time& dt)
		{
			SDL_Event tEvent;
			while (SDL_PollEvent(&tEvent))
			{
				switch (tEvent.type)
				{
					//QUIT event
				case SDL_QUIT:
					this->m_bQuit = true;
					return; //NOTE if we get a quit event, then we want to exit ASAP!
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
					this->HandleWindowEvent(tEvent, dt);
					break;
				case SDL_SYSWMEVENT:	/*Disabled by default, system specific events, to enable call SDL_EventState*/
					break;
					//KEYBOARD event
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					this->HandleKeyEvent(tEvent, dt);
					break;
					//TEXT event
				case SDL_TEXTEDITING:
				case SDL_TEXTINPUT:
					//MOUSE event
					break;
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEWHEEL:
					this->HandleMouseEvent(tEvent, dt);
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
		}

		EVENT_CALLBACK(CInput, OnRequestResolution, CRequestResolution)
		{
			event.curr_res = this->m_tCurrRes;
			event.default_res = this->m_tDefaultRes;
			event.scale = this->m_tScreenScale;
			return false;
		}

		void CInput::OnResolutionChange()
		{
			//Get current screen size
			SDL_Rect rect;
			rect.x = rect.y = 0;
			SDL_GetRendererOutputSize(this->m_pRen, &rect.w, &rect.h);

			this->m_tScreenScale = vec2(float(rect.w) / this->m_tDefaultRes.w, float(rect.h) / this->m_tDefaultRes.h);

			CResolutionChange eve;
			eve.old_res = this->m_tCurrRes;
			eve.scale = this->m_tScreenScale;
			eve.new_res = this->m_tCurrRes = Type2<int>(rect.w, rect.h);
			eve.Broadcast();
		}

		void CInput::HandleWindowEvent(const SDL_Event& tEvent, const Time& dt)
		{
			switch (tEvent.window.event)
			{
			case SDL_WINDOWEVENT_NONE:
				break;
			case SDL_WINDOWEVENT_SHOWN:
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				break;
			case SDL_WINDOWEVENT_MOVED:
				break;
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				this->OnResolutionChange();
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				break;
			case SDL_WINDOWEVENT_RESTORED:
				break;
			case SDL_WINDOWEVENT_ENTER:
				break;
			case SDL_WINDOWEVENT_LEAVE:
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;
			case SDL_WINDOWEVENT_CLOSE:
				break;
			}
		}

		void CInput::HandleKeyEvent(const SDL_Event& tEvent, const Time& dt)
		{
			switch (tEvent.type)
			{
			case SDL_KEYDOWN:
			{
				CKeyDownEvent eve;
				eve.code = tEvent.key.keysym.sym;
				eve.repeat = tEvent.key.state != 0;
				eve.Broadcast();
				break;
			}
			case SDL_KEYUP:
			{
				CKeyUpEvent eve;
				eve.code = tEvent.key.keysym.sym;
				eve.repeat = tEvent.key.state != 0;
				eve.Broadcast();
				break;
			}
			}
		}

		void CInput::HandleMouseEvent(const SDL_Event& tEvent, const Time& dt)
		{
			switch (tEvent.type)
			{
			case SDL_MOUSEMOTION:
			{
				CMouseMotionEvent eve;
				eve.pixel = Type2<slong>(tEvent.motion.x, this->m_tCurrRes.h - tEvent.motion.y);
				eve.delta = Type2<slong>(tEvent.motion.xrel, -tEvent.motion.yrel);
				eve.velocity = Type2<float>(float(eve.delta.x), float(eve.delta.y)) * this->m_fMouseElapsedTime;
				eve.layer_pos = FillOutLayerPos(eve.pixel);
				eve.Broadcast();
				this->m_fMouseElapsedTime = 0.0f;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				CMouseButtonDownEvent eve;
				eve.pixel = Type2<slong>(tEvent.motion.x, this->m_tCurrRes.h - tEvent.motion.y);
				eve.button = tEvent.button.button;
				eve.clicks = tEvent.button.clicks;
				eve.layer_pos = FillOutLayerPos(eve.pixel);
				eve.Broadcast();
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				CMouseButtonUpEvent eve;
				eve.pixel = Type2<slong>(tEvent.motion.x, this->m_tCurrRes.h - tEvent.motion.y);
				eve.button = tEvent.button.button;
				eve.clicks = tEvent.button.clicks;
				eve.layer_pos = FillOutLayerPos(eve.pixel);
				eve.Broadcast();
			}
			case SDL_MOUSEWHEEL:
				CMouseWheelEvent eve;
				eve.scroll = Type2<slong>(tEvent.wheel.x, tEvent.wheel.y);
				eve.Broadcast();
				break;
			}
		}

		void CInput::HandleTouchEvent(const SDL_Event& tEvent, const Time& dt)
		{
		}

		void CInput::HandleTextEvent(const SDL_Event& tEvent, const Time& dt)
		{
		}

		void CInput::HandleControllerEvent(const SDL_Event& tEvent, const Time& dt)
		{
		}

		void CInput::HandleJoyEvent(const SDL_Event& tEvent, const Time& dt)
		{
		}

		void CInput::HandleAppEvent(const SDL_Event& tEvent, const Time& dt)
		{

		}

		std::map < std::string, Type2<slong>> CInput::FillOutLayerPos(Type2<slong> pixel)
		{
			//TODO an optimization for this would be to figure this out in the responders where we actually care about location, this way
			//we don't do it every frame if we don't have to
			std::map < std::string, Type2<slong>> ret;
			const auto& list = Render::CRenderer::Get()->GetLayerList();
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				//All of our positions are based on the origin (center of the screen)
				Type2<slong> temp = pixel;
				temp.x -= m_tCurrRes.x * 0.5f;
				temp.y -= m_tCurrRes.y * 0.5f;

				//TODO we need to fix the inverse matrix func, we really should be using it here
				//const CMatrix2D& inv_mat = Math::Matrix_Inverse((*iter).second->GetCamera()->GetMatrix());
				//Type2<float> pos = Math::Vector_Matrix_Multiply(Type2<float>(temp.x, temp.y),inv_mat);

				const CMatrix2D& mat = (*iter).second->GetCamera()->GetMatrix();
				Type2<float> pos = Type2<float>(temp.x, temp.y) + mat.GetPosition();
				
				ret.insert(std::pair<std::string, Type2<slong>>((*iter).first, Type2<slong>((slong)pos.x, (slong)pos.y)));
			}
			return ret;
		}
	}
}