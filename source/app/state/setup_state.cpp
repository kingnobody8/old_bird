#include "setup_state.h"

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
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}