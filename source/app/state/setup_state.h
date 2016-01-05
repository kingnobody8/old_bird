#pragma once
#include "engine/state/base_state.h"

namespace app
{
	namespace state
	{
		class SetupState : public engine::IBaseState
		{
		private:

		public:
			SetupState();
			~SetupState();

			virtual void Init();
			virtual void Exit();
		};
	}
}