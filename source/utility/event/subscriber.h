#pragma once
#include <list>

namespace util
{
	namespace event
	{
		//forward declare
		class IPublisher;
		template<typename TYPE>
		class Publisher;

		class Subscriber
		{
			friend class VoidPublisher;
			template<typename TYPE>
			friend class Publisher;

		public:
			~Subscriber();
			void UnsubscribeAll();

		private:
			void AddPublisher(IPublisher* pPub);
			void RemPublisher(IPublisher* pPub);

			std::list<IPublisher*> m_pubs;
		};
	}
}