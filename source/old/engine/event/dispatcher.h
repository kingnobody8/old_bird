#pragma once
#include "engine/event/publisher.h"
#include "engine/util/util.h"
#include "engine/asdf.h"

namespace Engine
{
	namespace Event
	{
		//forward declare
		struct IEventBase;

		struct PubContainer
		{
			long long id = 0;
			CPublisher<IEventBase*>* pub = null;
		};
		struct EventDelayContainer
		{
			Util::Time	m_fDelay = -1;
			IEventBase* m_pEvent = null;
			EventDelayContainer(){}
			EventDelayContainer(IEventBase* const pEvent, const Util::Time& fDelay)
			{
				this->m_pEvent = pEvent;
				this->m_fDelay = fDelay;
			}
		};
		struct FuncDelayContainer
		{
			Util::Time	m_fDelay = -1;
			std::function<void(void)> m_func;
			FuncDelayContainer(){}
			FuncDelayContainer(std::function<void(void)> func, const Util::Time& fDelay)
			{
				this->m_func = func;
				this->m_fDelay = fDelay;
			}
		};

		class CDispatcher
		{
		private:
			std::vector<PubContainer> m_vPublishers;
			std::vector<EventDelayContainer> m_vDelayedEvents;
			std::vector<FuncDelayContainer> m_vDelayedFunctions;
			static CDispatcher* pInstance;

		private:
			CDispatcher(void){}
			~CDispatcher(void){}

		public:
			static CDispatcher* Get();
			static void DeleteInstance();

			void Update(Util::Time dt);

			void Broadcast(IEventBase* pEvent);
			void BroadcastDelay(IEventBase* pEvent, Util::Time fDelay);

			void Subscribe(IEventBase* pEvent, void* addr, std::function<bool(IEventBase*)> func); //TODO (daniel) add a priority to subscriptions?
			void Unsubscribe(IEventBase* pEvent, void* addr);
			void UnsubscribeAll(void* addr);
			void ClearEvent(IEventBase* pEvent);
			void ClearAll(void);

			void CallbackDelay(std::function<void(void)> func, Util::Time fDelay);
		};
	}
}