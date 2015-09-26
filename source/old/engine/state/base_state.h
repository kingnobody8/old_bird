/*
Author:		Daniel Habig
Date:		7-16-14
File:		Game State.h
Purpose:	Abstract Base Class for all types of states to inherit from
*/
#pragma once
#include "../util/util.h"
using namespace Engine::Util;
#include <string>
#include "../event/event.h"

namespace Engine
{
	namespace Event
	{
		class CPushStateEvent : public Event::IEvent
		{
		public:
			std::string		szName;
		};
	}

	namespace StateMachine
	{
		class IBaseState
		{
			/*Data*/
		private:
		protected:
			std::string		m_szName;
			std::string		m_szPrevStateName;
		public:

			//Func
		private:
		protected:
		public:
			IBaseState(void);
			virtual ~IBaseState(void) = 0;

			//Interface
			virtual void Init(void);				//Called when state is first pushed onto state machine stack, load and set here
			virtual void Update(Time fDelta);		//Called once every frame
			virtual void Exit(void);				//Called when the state being deleted, unload here
			virtual void Pause(void);				//Called when we our game is about to lose cpu for a while
			virtual void Resume(void);				//Called when cpu is returned

			// Gets
			inline	std::string		GetName(void)	const				{ return this->m_szName; }
			//Sets
			inline void				SetPrevState(const char* szPrev)	{ this->m_szPrevStateName = szPrev; }
		};
	}
}