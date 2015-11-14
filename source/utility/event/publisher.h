#pragma once
#include "../func.h"
#include "../macro.h"

namespace util
{
	namespace event
	{
		__todo() //figure this out when you actually have an game loop and input

		////forward declare
		//class Subscriber;

		////Base
		//template<typename Arg>
		//class Subscription
		//{
		//public:
		//	Subscription(Subscriber* sub);
		//	virtual ~Subscription() {};
		//	virtual void AddCallback(std::function<Arg> callback);
		//	virtual void Call(Arg arg) const = 0;
		//	Subscriber* GetSubscriber() const { return m_subscriber; }

		//protected:
		//	std::list<std::function<Arg>> m_callbacks;
		//	Subscriber* m_subscriber;
		//};

		////Void 
		//template<>
		//class Subscription<void>
		//{
		//public:
		//	Subscription(Subscriber* sub);
		//	virtual ~Subscription() {}
		//	virtual void AddCallback(std::function<void> callback);
		//	virtual void Call() const = 0;
		//	Subscriber* GetSubscriber() const { return m_subscriber; }

		//protected:
		//	std::list<std::function<void>> m_callbacks;
		//	Subscriber* m_subscriber;
		//};

	}
}