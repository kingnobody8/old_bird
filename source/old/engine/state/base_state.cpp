#include "base_state.h"

namespace Engine
{
	namespace StateMachine
	{
		IBaseState::IBaseState(void)
		{
			this->m_szName = "UNKOWN";
		}

		IBaseState::~IBaseState(void)
		{
			/*LEAVE EMPTY - ABC*/
		}

		/*virtual*/ void IBaseState::Init(void)
		{
		}
		/*virtual*/ void IBaseState::Update(Time fDelta)
		{
		}
		/*virtual*/ void IBaseState::Exit(void)
		{
		}
		/*virtual*/ void IBaseState::Pause(void)
		{
		}
		/*virtual*/ void IBaseState::Resume(void)
		{
		}
	}
}
