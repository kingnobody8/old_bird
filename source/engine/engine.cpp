#include "engine.h"
#include "component/object.h"
#include "component/part.h"
#include "script/script.h"
#include "render/renderer.h"
#include "input/input.h"
#include "state/base_state.h"

#include "plugin.h"
#include "state/state_plugin.h"

__todo() //why in God's name does this have to be not a class function. why won't SDL_SetIphoneANimation take a binded function like normal AHHHHHH!
void IosCallback(void* params)
{
	engine::Get()->RunFrame(params);
}

namespace engine
{
	Engine* Engine::s_pInstance = nullptr;
	Engine* Engine::Get(void)
	{
		if (!s_pInstance)
			s_pInstance = new Engine();
		return s_pInstance;
	}
	void Engine::DeleteInstance(void)
	{
		SafeDelete(s_pInstance);
	}

	Engine::Engine()
		: m_quit(true)
		, m_pRoot(null)
	{
	}
	Engine::~Engine()
	{
	}

	void Engine::Init(state::IBaseState* const pFirstState)
	{
		util::TypeCheck();

		//Register all engine scripts
		script::RegisterScripts();

		assert(pFirstState);

		//Init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
#ifdef MOBILE
			util::CheckSdlError("SDL not built with haptic (force feedback) support");
#else
			util::CheckSdlError();
#endif
		}

		//Turn off SDL audio
		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		//Initialize the rendering system
		render::Setup();

		//Init the input system
		input::Setup(render::GetSdlWindow());

		__todo()
			//Initialize box2d
			//script::box::IBox2DPart::SetupWorld();

			__todo()
			//Init Asset
			//Asset::CAsset::Get()->SetSdlRenderer(Render::CRenderer::Get()->GetRen());


		//Init state plugin
		state::StatePlugin* pStatePlugin = new state::StatePlugin();
		pStatePlugin->TransitionState(pFirstState);
		IPlugin::AddPlugin(pStatePlugin);

		//Start timer
		this->m_timer.Restart();

		//Don't quit
		m_quit = false;

#ifdef MOBILE
		//Set Render Callback
		SDL_iPhoneSetAnimationCallback(render::GetSdlWindow(), 1, IosCallback, nullptr);
		//SDL_SetI(render::GetSdlWindow(), 1, [this](void* params)
		//{
		//	this->Render();
		//});
		//SDL_iPhoneSetAnimationCallback(pWin, 1, render_callback, this);
#endif
	}

	void Engine::Exit(void)
	{
		IPlugin::DestroyPlugins();

		//Destroy Scene
		if (this->m_pRoot)
			this->m_pRoot->Destroy();
		this->m_pRoot = null;
		component::CObject::Clean();

		render::Destroy();

		//script::box::IBox2DPart::DestroyWorld();

		//Engine::Box::CBox::DeleteInstance();

		//Destroy Assets
		//Asset::CAsset::DeleteInstance();

		//Quit SDL
		SDL_Quit();

		DeleteInstance();
	}

	void Engine::RunFrame(void* params)
	{
		this->Update();
		render::DoRender();
	}

	void Engine::Update(void)
	{
		if (m_quit)
			return;

		//Update the timer
		this->m_timer.Signal();
		util::Time delta = this->m_timer.Delta();

		IPlugin::UpdatePlugins(delta);

		//script::box::IBox2DPart::UpdateWorld(delta);

		component::CObject::Clean(); //clean object graph

		//Poll events
		if (!input::PollSdl(delta))
		{
			m_quit = true;
		}
		else //if input did not kill the app, then update the current state and scene
		{
			component::IPart::UpdateParts(delta);
		}

		component::CObject::Clean(); //clean object graph
	}

}