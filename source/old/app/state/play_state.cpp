#include "play_state.h"
#include "engine/component/object.h"
#include "engine/event/dispatcher.h"
#include "engine/asset/loaders.h"
#include "engine/engine.h"
#include "engine/asset/resource_path.h"
#include "engine/render/render_node.h"

namespace App
{
	namespace StateMachine
	{
		CPlayState::CPlayState(void)
		{
			this->m_szName = "Intro_State";
		}
		CPlayState::~CPlayState(void)
		{
		}

		/*virtual*/ void CPlayState::Init(void)
		{
			IBaseState::Init();
			this->InitScene();
		}
		/*virtual*/ void CPlayState::Exit(void)
		{
			IBaseState::Exit();
			Engine::CUnsetRootEvent().Broadcast();
		}

		/*virtual*/ void CPlayState::Update(Time fDelta)
		{
		}

		void CPlayState::InitScene(void)
		{
			Engine::Object::CGroup* pRoot = new Engine::Object::CGroup();
			
			rapidjson::Document doc = Engine::Asset::FileLoaderJson(getResourcePath() + "assets/prefab/play.json");
			Engine::Object::CObject* pObj = Engine::Object::LoadObjectFromJson(doc);
			pRoot->AppendObject(pObj);
			
			Engine::CSetRootEvent().Broadcast(pRoot);
		}
	}
}