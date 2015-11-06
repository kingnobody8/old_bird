#pragma once
#include "timer.h"
#include "script/script.h"

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

	public:
		static Engine* Get(void);
		static void DeleteInstance(void);
		const bool GetQuit() const { return m_quit; }

		void Init(IBaseState* const pFirstState);
		void Exit(void);
		void RunFrame(void* params);

	};

	static Engine* Get()
	{
		return Engine::Get();
	}
}