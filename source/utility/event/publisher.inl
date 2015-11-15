#include "publisher.h"

namespace util
{
	namespace event
	{
		template<typename TYPE>
		void Publisher<TYPE>::Publish(TYPE arg)
		{
			assert(!m_publishing);
			m_publishing = true;

			if (!m_pending.empty())
			{
				for (int i = 0; i < m_pending.size(); ++i)
				{
					m_subscriptions.push_back(m_pending[i]);
				}
				m_pending.clear();
			}

			m_iter = m_subscriptions.begin();
			while (m_iter != m_subscriptions.end())
			{
				(*m_iter).second(arg);
				if (!m_erasure)
					++m_iter;
			}
			m_publishing = false;
		}

		template<typename TYPE>
		void Publisher<TYPE>::Subscribe(Subscriber* subscriber, Callback callback)
		{
			assert(subscriber);
			assert(callback != null);

			Subscription subscription(subscriber, callback);
			m_pending.push_back(subscription);

			subscriber->AddPublisher(this);
		}

		template<typename TYPE>
		void Publisher<TYPE>::Unsubscribe(Subscriber* subscriber)
		{
			for (int i = 0; i < m_pending.size(); ++i)
			{
				if (m_pending[i].first == subscriber)
				{
					subscriber->RemPublisher(this);
					m_pending.erase(m_pending.begin() + i);
					--i;
				}
			}

			SubIter local_iter = m_subscriptions.begin();
			while (local_iter != m_subscriptions.end())
			{
				if ((*local_iter).first == subscriber)
				{
					//check for publishing and current iterator
					if (m_publishing && m_iter == local_iter)
					{
						m_erasure = true;
					}
					(*local_iter).first->RemPublisher(this);
					local_iter = m_subscriptions.erase(local_iter);
					if (m_erasure)
					{
						m_iter = local_iter;
					}
				}
				else
				{
					++local_iter;
				}
			}
		}
	}
}