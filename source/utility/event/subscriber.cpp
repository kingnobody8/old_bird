#include "subscriber.h"
#include "publisher.h"

namespace util
{
	namespace event
	{
		Subscriber::Subscriber(void* addr)
			: m_addr(addr)
		{
		}

		Subscriber::~Subscriber()
		{
			UnsubscribeAll();
		}

		/*void Subscriber::UnsubscribeAll()
		{
			while (!m_pubs.empty())
			{
				Publisher* pub = m_pubs.back();
				m_pubs.pop_back();
				pub->Unsubscribe(this);
			}
		}*/

		void Subscriber::AddPublisher(Publisher* pPub)
		{
			m_pubs.push_back(pPub);
		}

		void Subscriber::RemPublisher(Publisher* pPub)
		{
			m_pubs.remove(pPub);
		}
	}
}