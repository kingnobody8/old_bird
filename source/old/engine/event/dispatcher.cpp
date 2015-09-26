#include "engine/event/dispatcher.h"
#include "engine/event/event.h"

namespace Engine
{
	namespace Event
	{
		CDispatcher* CDispatcher::pInstance = nullptr;
		CDispatcher* CDispatcher::Get(void)
		{
			if (!pInstance)
				pInstance = new CDispatcher();
			return pInstance;
		}
		void CDispatcher::DeleteInstance(void)
		{
			if (pInstance)
			{
				pInstance->ClearAll();
				delete pInstance;
				pInstance = nullptr;
			}
		}

		void CDispatcher::Broadcast(IEventBase* pEvent)
		{
			//Get the id of the event
			auto id = typeid(*pEvent).hash_code();

			//Find the publisher for that event
			CPublisher<IEventBase*>* pub = null;
			int size = this->m_vPublishers.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vPublishers[i].id == id)
				{
					pub = this->m_vPublishers[i].pub;
					break;
				}
			}

			//If a publisher exists, then publish the event, otherwise no one is listening for that event
			if (pub)
			{
				pub->Publish(pEvent);
			}
		}
		void CDispatcher::BroadcastDelay(IEventBase* pEvent, Util::Time fDelay)
		{
			this->m_vDelayedEvents.push_back(EventDelayContainer(pEvent, fDelay));
		}

		void CDispatcher::Update(Util::Time dt)
		{
			//Delayed Events
			for (int i = 0; i < (signed)this->m_vDelayedEvents.size(); ++i)
			{
				EventDelayContainer* pIdx = &this->m_vDelayedEvents[i];
				pIdx->m_fDelay -= dt;

				if (pIdx->m_fDelay < 0)
				{
					this->Broadcast(pIdx->m_pEvent);
					delete pIdx->m_pEvent;
					this->m_vDelayedEvents.erase(this->m_vDelayedEvents.begin() + i);
					--i;
				}
			}

			//Delayed Function
			for (int i = 0; i < (signed)this->m_vDelayedFunctions.size(); ++i)
			{
				FuncDelayContainer* pIdx = &this->m_vDelayedFunctions[i];
				pIdx->m_fDelay -= dt;

				if (pIdx->m_fDelay < 0)
				{
					pIdx->m_func();
					this->m_vDelayedFunctions.erase(this->m_vDelayedFunctions.begin() + i);
					--i;
				}
			}
		}

		void CDispatcher::Subscribe(IEventBase* pEvent, void* addr, std::function<bool(IEventBase*)> func)
		{
			assert(pEvent);

			//Get event id
			auto id = typeid(*pEvent).hash_code();

			//Find a publisher for that event
			CPublisher<IEventBase*>* pPub = null;
			for (int i = 0, size = this->m_vPublishers.size(); i < size; ++i)
			{
				if (this->m_vPublishers[i].id == id)
				{
					pPub = this->m_vPublishers[i].pub;
					break;
				}
			}

			//If the publisher doesn't exist then make a new one
			if (!pPub)
			{
				PubContainer temp;
				temp.id = id;
				temp.pub = new CPublisher<IEventBase*>();
				this->m_vPublishers.push_back(temp);
				pPub = temp.pub;
			}

			//Find if a subscriber matches the addr
			ISubscriber<bool, IEventBase*>* pSub = pPub->FindSubscriberByAddr(addr);

			//If no subscriber exists, make a new one
			if (!pSub)
			{
				pSub = new CSubscriber<bool, IEventBase*>();
				pSub->SetAddr(addr);
				pPub->AddSubscriber(pSub);
			}

			//Add the function to the subscriber
			pSub->AddFunction(func);
		}
		void CDispatcher::Unsubscribe(IEventBase* pEvent, void* addr)
		{
			assert(pEvent);

			//Get the event id
			auto id = typeid(*pEvent).hash_code();

			//Find the publisher for that event
			int size = this->m_vPublishers.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vPublishers[i].id == id)
				{
					this->m_vPublishers[i].pub->RemByAddr(addr);
					return;
				}
			}
		}
		void CDispatcher::UnsubscribeAll(void* addr)
		{
			int size = this->m_vPublishers.size();
			for (int i = 0; i < size; ++i)
			{
				this->m_vPublishers[i].pub->RemByAddr(addr);
			}
		}
		void CDispatcher::ClearEvent(IEventBase* pEvent)
		{
			//Get the event id
			auto id = typeid(*pEvent).hash_code();

			//Find the publisher for that event
			int size = this->m_vPublishers.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vPublishers[i].id == id)
				{
					delete this->m_vPublishers[i].pub;
					this->m_vPublishers.erase(this->m_vPublishers.begin() + i);
					return;
				}
			}
		}
		void CDispatcher::ClearAll(void)
		{
			int size = this->m_vPublishers.size();
			for (int i = 0; i < size; ++i)
			{
				delete this->m_vPublishers[i].pub;
			}
			this->m_vPublishers.clear();
		}

		void CDispatcher::CallbackDelay(std::function<void(void)> func, Util::Time fDelay)
		{
			this->m_vDelayedFunctions.push_back(FuncDelayContainer(func, fDelay));
		}
	}
}