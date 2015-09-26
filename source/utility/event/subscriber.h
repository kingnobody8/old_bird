#pragma once
#include <list>

namespace util
{
	namespace event
	{
		//forward declare
		class Publisher;

		class Subscriber
		{
		public:
			Subscriber(void* addr);
			~Subscriber();
			void UnsubscribeAll();

		private:
			void AddPublisher(Publisher* pPub);
			void RemPublisher(Publisher* pPub);

			std::list<Publisher*> m_pubs;
			void* m_addr;
		};
	}
}