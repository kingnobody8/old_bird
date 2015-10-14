#include "engine.h"
#include "func.h"
#include "component/object.h"
#include <assert.h>
#include "render/renderer.h"
#include "asset/loader.h"
#include "asset/resource_path.h"
#include "render/render_layer.h"
#include "component/part.h"

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
		if (s_pInstance)
		{
			delete s_pInstance;
			s_pInstance = nullptr;
		}
	}

	Engine::Engine()
		: m_quit(true)
		, m_pCurrState(null)
		, m_pNextState(null)
		, m_pRoot(null)
	{
	}
	Engine::~Engine()
	{
	}

	void Engine::Init(IBaseState* const pFirstState)
	{
		util::TypeCheck();

		//assert(pFirstState);

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
		render::SetupSdl();
		render::CRenderLayer::CreateLayer("ui_layer", 0, null);
		__todo() //setup render layers here?
		//Render::CRenderer::Get()->Init();

#ifdef MOBILE
		//Set Render Callback
		SDL_iPhoneSetAnimationCallback(render::GetSdlWindow(), 1, IosCallback,nullptr);
		//SDL_SetI(render::GetSdlWindow(), 1, [this](void* params)
		//{
		//	this->Render();
		//});
		//SDL_iPhoneSetAnimationCallback(pWin, 1, render_callback, this);
#endif

		//Init Asset
		__todo()
	//	Asset::CAsset::Get()->SetSdlRenderer(Render::CRenderer::Get()->GetRen());

		//Init Input
		__todo()
		//this->m_cInput.Init(Render::CRenderer::Get()->GetRen());

		//Push the first state
		//this->PushState(pFirstState);

		//Start timer
		this->m_timer.Restart();

		util::JSON doc = asset::FileLoaderJson(getResourcePath() + "assets/test.json");
		m_pRoot = static_cast<component::CGroup*>(component::LoadObjectFromJson(doc));
		component::PartList parts = component::GetPartList(m_pRoot);
		for (component::PartIter iter = parts.begin(); iter != parts.end(); ++iter)
		{
			(*iter)->Init();
		}

		//Don't quit
		m_quit = false;
	}
	void Engine::Exit(void)
	{
		//Destroy Scene
		if (this->m_pRoot)
			this->m_pRoot->Destroy();
		this->m_pRoot = null;

		////Destroy State
		//if (this->m_pCurrState)
		//{
		//	this->m_pCurrState->Exit();
		//	delete this->m_pCurrState;
		//	this->m_pCurrState = null;
		//}

		__todo() //delete render layers here?
		render::CRenderLayer::DestroyLayers();
		//Destroy Renderer
		//Render::CRenderer::DeleteInstance();

		//Engine::Box::CBox::DeleteInstance();

		//Destroy Renderer
		//this->m_cRenderer.Exit();

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

		//If a new state is available, then change states
		if (this->m_pNextState)
			this->PushState(this->m_pNextState);

		//Update the timer
		this->m_timer.Signal();
		util::Time delta = this->m_timer.Delta();
		
		__todo()//remove this, this was for testing only
		util::math::Matrix2D mat = m_pRoot->GetLocalMatrix();
		float use = (this->m_timer.Total().Milli() % 1000) / 1000.0f;
		mat.SetScale(util::math::vec2(use, 1.0f));
		mat.SetRotationZ(use * 300);
		m_pRoot->SetLocalMatrix(mat);

		//Poll events
		SDL_Event tEvent;
		while (SDL_PollEvent(&tEvent))
		{
			switch (tEvent.type)
			{
			case SDL_QUIT:
				this->m_quit = true;
			}
		}

		////If input did not kill the app, then update the current state and the scene
		//this->m_bRun = !this->m_cInput.Update(delta);
		//if (this->m_bRun)
		//{
		//	Box::CBox::Get()->Update(delta);
		//	if (this->m_pCurrState)
		//		this->m_pCurrState->Update(delta);

		//	Part::IPart::UpdateParts(delta);
		//}
	}
	
	void Engine::PushState(IBaseState* pState)
	{
		assert(pState);

		/*std::string prev_state;
		this->m_pNextState = null;
		if (this->m_pCurrState)
		{
			prev_state = this->m_pCurrState->GetName();
			this->m_pCurrState->Exit();
			delete this->m_pCurrState;
			this->m_pCurrState = null;
		}
		this->m_pCurrState = pState;
		this->m_pCurrState->SetPrevState(prev_state.c_str());
		this->m_pCurrState->Init();*/
	}

}