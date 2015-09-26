#include "engine/engine.h"
#include "engine/platform/inc_sdl.h"
#include "engine/util/func.h"
#include "engine/event/dispatcher.h"
#include "engine/asset/asset.h"
#include "engine/component/object.h"
#include "engine/state/base_state.h"
#include "engine/box/box.h"
#include "engine/component/part.h"
#include "engine/box/box.h"
#include "engine/render/renderer.h"

void render_callback(void* param)
{
	Engine::CEngine* pEngine = reinterpret_cast<Engine::CEngine*>(param);
	pEngine->Update();
}

namespace Engine
{
	CEngine* Get(void)
	{
		return CEngine::Get();
	}

	CEngine* CEngine::instance = nullptr;
	CEngine* CEngine::Get(void)
	{
		if (!instance)
			instance = new CEngine();
		return instance;
	}
	void CEngine::DeleteInstance(void)
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	CEngine::CEngine(void)
	{
		this->m_pCurrState = this->m_pNextState = null;
		this->m_pDispatch = null;
		this->m_pRoot = null;
		this->m_bRun = false;
	}
	CEngine::~CEngine(void)
	{
	}

	void CEngine::Init(StateMachine::IBaseState* const pFirstState)
	{
		Util::TypeCheck();

		//assert(pFirstState);

		//Init Dispatcher
		this->m_pDispatch = Engine::Event::CDispatcher::Get();

		//Subscribe to Engine Events
		CSetStateEvent().Subscribe(this, BIND1(this, &CEngine::OnSetState));
		CSetRootEvent().Subscribe(this, BIND1(this, &CEngine::OnSetRoot));
		CUnsetRootEvent().Subscribe(this, BIND1(this, &CEngine::OnUnsetRoot));
		//Input::CMouseButtonDownEvent().Subscribe(this, BIND1(this, &CEngine::Test));

		//Init timer
		this->m_cTimer.Restart();

		//Init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
#ifdef MOBILE
			std::string error = SDL_GetError();
			SDL_Log("Error - SDL_Init - %s.\n", error.c_str());
			//assert(error != "SDL not built with haptic (force feedback) support");
#else
			SDL_Log("Error - SDL_Init - %s.\n", SDL_GetError());
			//assert(false);
#endif
		}

		//Turn off SDL audio
		SDL_QuitSubSystem(SDL_INIT_AUDIO);

		//Initialize the rendering system
		Render::CRenderer::Get()->Init();

#ifdef MOBILE
		//Set Render Callback
		//SDL_iPhoneSetAnimationCallback(pWin, 1, [this](void* params)
		//{
		//	this->Render();
		//});
		SDL_iPhoneSetAnimationCallback(pWin, 1, render_callback, this);
#endif

		//Init Asset
		Asset::CAsset::Get()->SetSdlRenderer(Render::CRenderer::Get()->GetRen());

		//Init Input
		this->m_cInput.Init(Render::CRenderer::Get()->GetRen());

		//Start
		this->m_bRun = true;

		//Push the first state
		this->PushState(pFirstState);
	}
	void CEngine::Run(void)
	{
#ifdef PC
		while (this->m_bRun)
		{
			this->Update();
		}
#endif
	}
	void CEngine::Exit(void)
	{
		//Destroy Scene
		if (this->m_pRoot)
			this->m_pRoot->Destroy();
		this->m_pRoot = null;

		//Destroy State
		if (this->m_pCurrState)
		{
			this->m_pCurrState->Exit();
			delete this->m_pCurrState;
			this->m_pCurrState = null;
		}

		//Destroy Renderer
		Render::CRenderer::DeleteInstance();

		Engine::Box::CBox::DeleteInstance();

		//Destroy Dispatch
		this->m_pDispatch->DeleteInstance();
		this->m_pDispatch = null;

		//Destroy Renderer
		//this->m_cRenderer.Exit();

		//Destroy Assets
		Asset::CAsset::DeleteInstance();

		//Quit SDL
		SDL_Quit();

		DeleteInstance();
	}

	void CEngine::Update(void)
	{
		//Update the timer
		this->m_cTimer.Signal();
		Time fDelta = this->m_cTimer.Delta();

		//If a new state is available, then set it
		if (this->m_pNextState)
			this->PushState(this->m_pNextState);

		//If input did not kill the app, then update the current state and the scene
		this->m_bRun = !this->m_cInput.Update(fDelta);
		if (this->m_bRun)
		{
			this->m_pDispatch->Update(fDelta);
			Box::CBox::Get()->Update(fDelta);
			if (this->m_pCurrState)
				this->m_pCurrState->Update(fDelta);

			Part::IPart::UpdateParts(fDelta);

			Render::CRenderer::Get()->Update(fDelta);
			this->Render();
		}
	}
	void CEngine::Render(void)
	{
		Render::CRenderer::Get()->Render(this->m_pRoot);
	}
	void CEngine::PushState(StateMachine::IBaseState* pState)
	{
		std::string prev_state;
		this->m_pNextState = null;
		if (this->m_pCurrState)
		{
			prev_state = this->m_pCurrState->GetName();
			this->m_pCurrState->Exit();
			delete this->m_pCurrState;
			this->m_pCurrState = null;
		}
		if (pState)
		{
			this->m_pCurrState = pState;
			this->m_pCurrState->SetPrevState(prev_state.c_str());
			this->m_pCurrState->Init();
		}
		else
		{
			SDL_Log("Warning - CEngine::PushState - null state passed in");
		}
	}

	EVENT_CALLBACK(CEngine, OnSetState, CSetStateEvent)
	{
		assert(event.data);
		if (this->m_pCurrState)
			this->m_pNextState = event.data;
		else
			this->PushState(event.data);
		return false;
	}

	EVENT_CALLBACK(CEngine, OnSetRoot, CSetRootEvent)
	{
		assert(event.data);

		if (this->m_pRoot)
		{
			this->m_pRoot->Exit();
			this->m_pRoot->Destroy();
		}

		this->m_pRoot = event.data;
		//Init the parts
		std::list<Part::IPart*> list = Object::GetPartList(this->m_pRoot);
		for (auto iter = list.begin(); iter != list.end(); ++iter)
		{
			(*iter)->Init();
		}

		return false;
	}

	EVENT_CALLBACK(CEngine, OnUnsetRoot, CUnsetRootEvent)
	{
		if (this->m_pRoot)
		{
			this->m_pRoot->Exit();
			this->m_pRoot->Destroy();
		}
		this->m_pRoot = null;
		return false;
	}

	EVENT_CALLBACK(CEngine, Test, Engine::Input::CMouseButtonDownEvent)
	{
		if (event.button == SDL_BUTTON_LMASK && event.clicks > 2)
			Engine::Input::CMouseWheelEvent().Unsubscribe(this);
		return false;
	}
}