#pragma once
#include "engine/state/base_state.h"

namespace App
{
	namespace StateMachine
	{
		class CIntroState : public Engine::StateMachine::IBaseState
		{
			/*Data*/
		private:
		protected:
		public:

			/*Func*/
		private:
		protected:
			void InitScene(void);
		public:
			CIntroState(void);
			virtual ~CIntroState(void);

			virtual void	Init(void);
			virtual void	Exit(void);
			virtual void	Update(Time fDelta);
		};
	}
}