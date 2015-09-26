#pragma once
#include "engine/platform/inc_boost.h"
#include "engine/util/util.h"
#include <vector>

namespace Engine
{
	namespace Event
	{
		//Base
		class ISubscriberBase
		{
		protected:
			void* addr;

		public:
			ISubscriberBase(){ addr = null; }
			virtual ~ISubscriberBase() = 0;

			inline void SetAddr(void* pAddr) { this->addr = pAddr; }
			inline void* GetAddr(void) const { return this->addr; }
		};
		inline ISubscriberBase::~ISubscriberBase() { /*LEAVE EMPTY - ABC*/ }

		//Templated Base
		template<typename ReturnType, typename Param>
		class ISubscriber : public ISubscriberBase
		{
		protected:
			std::vector<std::function<ReturnType(Param)>> m_vFunctions;

		public:
			ISubscriber(){}
			inline void AddFunction(std::function<ReturnType(Param)> func)
			{
				this->m_vFunctions.push_back(func);
			}
			virtual void Call(Param) = 0;
		};

		//Explicit
		template <typename ReturnType, typename Param>
		class CSubscriber : public ISubscriber < ReturnType, Param >
		{
		public:
			CSubscriber(){}
			CSubscriber(void* addr, std::function<ReturnType(Param)> func)
			{
				this->addr = addr;
				this->m_vFunctions.push_back(func);
			}
			inline virtual void Call(Param param)
			{
				for (int i = 0, size = this->m_vFunctions.size(); i < size; ++i)
				{
					this->m_vFunctions[i](param);
				}
			}
		};

		//NOTE: (daniel) removed the partial specialization for void because mac/ios compiler doesn't support void as a parameter
		//Partial Specialization (the 'Call' function parameter is void)
		/*template<typename ReturnType>
		class CSubscriber<ReturnType, void> : public ISubscriber < ReturnType, void >
		{
		public:
			CSubscriber(){}
			CSubscriber(void* addr, std::function<ReturnType(void)> func)
			{
				this->addr = addr;
				this->m_vFunctions.push_back(func);
			}
			inline virtual void Call(void)
			{
				for (int i = 0, size = this->m_vFunctions.size(); i < size; ++i)
				{
					this->m_vFunctions[i]();
				}
			}
		};*/
	}
}
