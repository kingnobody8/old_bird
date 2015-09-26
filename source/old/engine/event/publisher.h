#pragma once
#include "engine/event/subscriber.h"
#include <vector>

namespace Engine
{
	namespace Event
	{
		//Base
		class IPublisherBase
		{
		public:
			IPublisherBase(){}
			virtual ~IPublisherBase() = 0;
		};
		inline IPublisherBase::~IPublisherBase() { /*LEAVE EMPTY - ABC*/ }

		//Templated Base
		template<typename Param>
		class IPublisher : public IPublisherBase
		{
		protected:
			std::vector<ISubscriber<bool, Param>*> m_vSubscribers;

		public:
			IPublisher(){}
			virtual ~IPublisher() = 0;

			inline void AddSubscriber(ISubscriber<bool, Param>* pSubscriber)
			{
				this->m_vSubscribers.push_back(pSubscriber);
			}
			inline void RemSubscriber(ISubscriber<bool, Param>* pSubscriber)
			{
				for (int i = 0, size = this->m_vSubscribers.size(); i < size; ++i)
				{
					if (this->m_vSubscribers[i] == pSubscriber)
					{
						delete this->m_vSubscribers[i];
						this->m_vSubscribers.erase(this->m_vSubscribers.begin() + i);
						break;
					}
				}
				return;
			}
			inline void RemByAddr(void* pAddr)
			{
				for (int i = 0, size = this->m_vSubscribers.size(); i < size; ++i)
				{
					if (this->m_vSubscribers[i]->GetAddr() == pAddr)
					{
						delete this->m_vSubscribers[i];
						this->m_vSubscribers.erase(this->m_vSubscribers.begin() + i);
						return;
					}
				}
			}
			inline ISubscriber<bool, Param>* FindSubscriberByAddr(void* addr)
			{
				for (int i = 0, size = this->m_vSubscribers.size(); i < size; ++i)
				{
					if (this->m_vSubscribers[i]->GetAddr() == addr)
					{
						return this->m_vSubscribers[i];
					}
				}
				return null;
			}
			inline void Clean(void)
			{
				int size = this->m_vSubscribers.size();
				for (int i = 0; i < size; ++i)
				{
					delete this->m_vSubscribers[i];
				}
				this->m_vSubscribers.clear();
			}
			virtual void Publish(Param) = 0;
		};
		template<typename Param>
		inline IPublisher<Param>::~IPublisher() { this->Clean(); }

		//Explicit
		template<typename Param>
		class CPublisher : public IPublisher < Param >
		{
		public:
			inline virtual void Publish(Param param)
			{
				for (int i = 0, size = this->m_vSubscribers.size(); i < size; ++i)
				{
					this->m_vSubscribers[i]->Call(param);
				}
			}
		};

		//NOTE: (daniel) removed the partial specialization for void because mac/ios compiler doesn't support void as a parameter
		////Partial Specialization (the 'Publish' function parameter is void)
		//template<>
		//class CPublisher<void> : public IPublisher < void >
		//{
		//public:
		//	inline virtual void Publish(void)
		//	{
		//		for (int i = 0, size = this->m_vSubscribers.size(); i < size; ++i)
		//		{
		//			this->m_vSubscribers[i]->Call();
		//		}
		//	}
		//};

	}
}