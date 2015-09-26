#include "intro_state.h"
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
		CIntroState::CIntroState(void)
		{
			this->m_szName = "Intro_State";
		}
		CIntroState::~CIntroState(void)
		{
		}

		/*virtual*/ void CIntroState::Init(void)
		{
			IBaseState::Init();
			this->InitScene();
		}
		/*virtual*/ void CIntroState::Exit(void)
		{
			IBaseState::Exit();
			Engine::CUnsetRootEvent().Broadcast();
		}

		/*virtual*/ void CIntroState::Update(Time fDelta)
		{
		}

		void CIntroState::InitScene(void)
		{
			Engine::Object::CGroup* pRoot = new Engine::Object::CGroup();

			rapidjson::Document doc = Engine::Asset::FileLoaderJson(getResourcePath() + "assets/prefab/ui/logo.prefab.json");
			Engine::Object::CObject* pObj = Engine::Object::LoadObjectFromJson(doc);
			pRoot->AppendObject(pObj);

			Engine::CSetRootEvent().Broadcast(pRoot);
		}
	}
}