#pragma once
#include "timer.h"
#include "script/script.h"

#include "event/publisher.h"
#include "event/publisher.inl"
#include "event/subscriber.h"
#include "input/input.h"

namespace engine
{
	//forward declare
	class IBaseState;
	class Engine;
	namespace component
	{
		class CObject;
		class CGroup;
		class IPart;
	}

	class Engine
	{
	private:
		util::CTimer m_timer;
		IBaseState* m_pCurrState;
		IBaseState* m_pNextState;
		component::CGroup*		m_pRoot;
		bool					m_quit;

		static Engine* s_pInstance;
		Engine();
		~Engine();

		void Update();
		void PushState(IBaseState* const pState);

		void OnMode(int mode);
		void OnMode2(int mode);
		void OnAKey(input::key_events::KeyAction action);
		void OnMouseBtn(input::mouse_events::MotionAction action);

	public:
		static Engine* Get(void);
		static void DeleteInstance(void);
		const bool GetQuit() const { return m_quit; }

		void Init(IBaseState* const pFirstState);
		void Exit(void);
		void RunFrame(void* params);


		util::event::IntPublisher pub;
		util::event::Subscriber sub;
	};

	static Engine* Get()
	{
		return Engine::Get();
	}
}