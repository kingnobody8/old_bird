#pragma once
#include "engine/event/dispatcher.h"

#define PROTO_EVENT_CALLBACK(FuncName, EventType)	\
bool FuncName(Engine::Event::IEventBase*);			\
bool FuncNameCall(EventType&);						\

#define EVENT_CALLBACK(Class, FuncName, EventType)		\
bool Class::FuncName(Engine::Event::IEventBase* event)	\
{														\
	EventType* pEvent = static_cast<EventType*>(event);	\
	return FuncNameCall(*pEvent);						\
}														\
bool Class::FuncNameCall(EventType& event)				\

namespace Engine
{
	namespace Event
	{
		struct IEventBase
		{
			virtual ~IEventBase() = 0;

			void Subscribe(void* addr, std::function<bool(IEventBase*)> func)
			{
				CDispatcher::Get()->Subscribe(this, addr, func);
			}
			void Unsubscribe(void* addr)
			{
				CDispatcher::Get()->Unsubscribe(this, addr);
			}
			void ClearEvent()
			{
				CDispatcher::Get()->ClearEvent(this);
			}
		};
		inline IEventBase::~IEventBase() { /*LEAVE EMPTY - ABC*/ }

		struct IEvent : public IEventBase
		{
			virtual ~IEvent() = 0;
			void Broadcast()
			{
				CDispatcher::Get()->Broadcast(this);
			}
		};
		inline IEvent::~IEvent() { /*LEAVE EMPTY - ABC*/ }


		template<typename type>
		struct IDataEvent : public IEventBase
		{
			type	data;

			virtual ~IDataEvent() = 0;

			void Broadcast(type data)
			{
				this->data = data;
				CDispatcher::Get()->Broadcast(this);
			}
		};
		template<typename type>
		inline IDataEvent<type>::~IDataEvent() { /*LEAVE EMPTY - ABC*/ }

	}
}
