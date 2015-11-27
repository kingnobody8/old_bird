#include "engine.h"
#include "func.h"
#include "component/object.h"
#include <assert.h>
#include "render/renderer.h"
#include "input/input.h"
#include "asset/loader.h"
#include "asset/resource_path.h"
#include "render/render_layer.h"
#include "render/camera.h"
#include "component/part.h"
#include "script/renderable/img_part.h"
#include "component/object.inl"
#include "script/box/box2d_part.h"

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

		//Init all engine scripts
		script::RegisterScripts();
		
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

		//Initialize the input system
		input::Setup(render::GetSdlRenderer());

		script::box::IBox2DPart::SetupWorld();

		util::event::Publisher<input::key_events::KeyAction>* ptr = &input::key_events::s_InputKeyUp;
		input::key_events::s_InputKeyUp.Subscribe(&sub, BIND1(this, &Engine::OnAKey));
		input::mouse_events::s_InputMouseMotion.Subscribe(&sub, BIND1(this, &Engine::OnMouseBtn));

		__todo() //refactor this into the initialization of the app class when it is
		util::JSON rconfig = asset::FileLoaderJson(getResourcePath() + "assets/config/render_config.json");
		for (int i = 0; i < (int)rconfig["camera_list"].Size(); ++i)
		{
			render::CCamera::CreateCamera(rconfig["camera_list"][i].GetString());
		}
		for (int i = 0; i < (int)rconfig["render_layer_list"].Size(); ++i)
		{
			std::string name = rconfig["render_layer_list"][i]["name"].GetString();
			std::string camera = rconfig["render_layer_list"][i]["camera"].GetString();
			render::CRenderLayer::CreateLayer(name, i, render::CCamera::FindCamera(camera));
		}

		//auto mat = cam->GetMatrix();
		//mat.SetScale(vec2(2.0f,1.0f));
	//	cam->SetMatrix(mat);
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

			if ((*iter)->GetType() == script::ui::CButtonPart::Type)
			{
				script::ui::CButtonPart* btn = (script::ui::CButtonPart*)(*iter);
				btn->Subscribe(&sub, BIND1(this, &Engine::OnBtn));
			}
		}

		//Don't quit
		m_quit = false;

		pub.Subscribe(&sub, BIND1(this, &Engine::OnMode));
		pub.Subscribe(&sub, BIND1(this, &Engine::OnMode2));
	}

	void Engine::OnMode(int mode)
	{
		component::CObject* obj = m_pRoot->FindObject("door");

		script::renderable::CImgPart* pimg = obj->FindPart<script::renderable::CImgPart>();
		if (pimg)
			pimg->SetBlendMode((SDL_BlendMode)(mode));

		//sub.UnsubscribeAll();
	}

	void Engine::OnMode2(int mode)
	{
		int x = 0;
		x++;
		pub.Unsubscribe(&sub);
	}

	void Engine::OnBtn(script::ui::CButtonPart* btn)
	{
		static int mode = 0;
		mode++;
		if (mode > 4)
			mode = 0;

		component::CObject* obj = m_pRoot->FindObject("door");

		script::renderable::CImgPart* pimg = obj->FindPart<script::renderable::CImgPart>();
		if (pimg)
			pimg->SetBlendMode((SDL_BlendMode)(mode));
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

		render::Destroy();

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

		script::box::IBox2DPart::UpdateWorld(delta);
		
		__todo()//remove this, this was for testing only
		component::CObject* obj = m_pRoot->FindObject("door");

		util::math::Matrix2D mat = obj->GetLocalMatrix();
		float use = (this->m_timer.Total().Milli() % 1000) / 1000.0f;
		//mat.SetScale(util::math::vec2(use, 1.0f));
		mat.SetRotationZ(this->m_timer.Total().Milli() / -10.0f);
		obj->SetLocalMatrix(mat);

		static int mode = 0;
		static util::Time timer;
		timer += m_timer.Delta();
		if (timer > 2000)
		{
			timer = 0;
			mode += 1;
			if (mode > 4)
				mode = 0;
			//script::renderable::CImgPart* pimg = obj->FindPart<script::renderable::CImgPart>();
			//if(pimg)
			//	pimg->SetBlendMode((SDL_BlendMode)mode);
			//pub.Publish(mode);
		}

		/*mat = Matrix2D();
		mat.SetPosition(vec2(this->m_timer.Total().Milli() / 10, 0.0f));
		render::CCamera::FindCamera("world_cam")->SetMatrix(mat);*/

		//Poll events
		if (!input::PollSdl(delta))
		{
			m_quit = true;
		}
		else //if input did not kill the app, then update the current state and scene
		{
			component::IPart::UpdateParts(delta);
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

	void Engine::OnAKey(input::key_events::KeyAction action)
	{
		m_quit = true;
	}

	void Engine::OnMouseBtn(input::mouse_events::MotionAction action)
	{
		/*if (action.m_delta.magnitude() > 30)
		{
			m_quit = true;
		}*/
	}
}