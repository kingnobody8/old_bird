#pragma once
#include "component/component.h"
#include "timer.h"

namespace engine
{
	__todo() //make the different 'systems' (component, input, render, physics) inherit from a base system class so that they can be inited and exited and updated the same
		//also let client inherit and create systems (physics) should probably be client

	//forward declare
	class Engine;
	namespace state { class IBaseState; }
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
		component::CGroup*		m_pRoot;
		bool					m_quit;

		static Engine* s_pInstance;
		Engine();
		~Engine();

		void Update();

	public:
		static Engine* Get(void);
		static void DeleteInstance(void);
		const bool GetQuit() const { return m_quit; }

		void Init(state::IBaseState* const pFirstState);
		void Exit(void);
		void RunFrame(void* params);
	};

	static Engine* Get()
	{
		return Engine::Get();
	}
}