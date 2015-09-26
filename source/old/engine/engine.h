#pragma once
#include "asdf.h"
#include "input/input.h"
#include "engine/input/input_event.h"

using namespace Engine::Util;

namespace Engine
{
	static vec2 default_dims = vec2(1080, 1920);
	static vec2 physical_dims = vec2(540, 960);

	//Forward Declare
	namespace StateMachine { class IBaseState; }
	namespace Object { class CObject; class CGroup; }
	namespace Event { class CDispatcher; }
	class CEngine;

	//EVENTS
	struct CSetStateEvent : public Event::IDataEvent < StateMachine::IBaseState* > {};
	struct CSetRootEvent : public Event::IDataEvent < Engine::Object::CGroup* > {};
	struct CUnsetRootEvent : public Event::IEvent {};

	CEngine* Get(void);

	class CEngine
	{
		/*Data*/
	private:
		CTimer m_cTimer;
		StateMachine::IBaseState* m_pCurrState;
		StateMachine::IBaseState* m_pNextState;
		Event::CDispatcher* m_pDispatch;
		Object::CGroup*		m_pRoot;
		bool				m_bRun;
		Input::CInput		m_cInput;


		/*Func*/
	private:
		static CEngine* instance;
		CEngine(void);
		~CEngine(void);

		//void Update(void);
		void PushState(StateMachine::IBaseState* pState);

		PROTO_EVENT_CALLBACK(OnSetState, CSetStateEvent);
		PROTO_EVENT_CALLBACK(OnSetRoot, CSetRootEvent);
		PROTO_EVENT_CALLBACK(OnUnsetRoot, CUnsetRootEvent);


	public:
		static CEngine* Get(void);
		static void DeleteInstance(void);

		void Update();
		void Render(void);
		void Init(StateMachine::IBaseState* const pFirstState);
		void Run(void);
		void Exit(void);


		PROTO_EVENT_CALLBACK(Test, Engine::Input::CMouseButtonDownEvent)
	};
}
