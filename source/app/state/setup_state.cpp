#include "setup_state.h"
#include "engine/state/state_plugin.h"

namespace app
{
	namespace state
	{
		SetupState::SetupState()
		{
			this->m_name = "SetupState";
		}

		VIRTUAL SetupState::~SetupState()
		{
		}

		VIRTUAL void SetupState::Init()
		{
			//test for initing anoter state within an init
			//engine::state::StatePlugin* plug = static_cast<engine::state::StatePlugin*>(engine::IPlugin::FindPlugin(engine::state::StatePlugin::Type));
			//plug->TransitionState(new SetupState());
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}