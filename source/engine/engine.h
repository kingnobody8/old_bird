#pragma once
#include "timer.h"
#include "render/render_plugin.h"

namespace engine
{
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
		render::RenderPlugin* m_pRenderPlugin;

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