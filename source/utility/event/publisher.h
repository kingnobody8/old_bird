#pragma once
#include "../func.h"
#include "../macro.h"
#include "util_time.h"
#include "json.h"
#include "types.h"
#include <list>
#include "subscriber.h"

namespace util
{
	namespace event
	{

		/*
			--Publisher Usage--
			In regards to RECURSION: don't do it, publisher will assert if a publish is called while already in the process of a publish
			In regards to SUBSCRIPTION: you may do this, even in the middle of a publish, however if this happens in the middle of a publish they will not be included in the current publish
			In regards to UNSUBSCRIPTION: you may do this, even in the middle of a publish. howevr if this happens in the middle of a publish and if the subscriber that is unsubscribing has not
			yet been called in the publish, it will still be removed and then will not be called
		*/

		//forward declare
		class Subscriber;

		//base publisher
		class IPublisher
		{
		public:
			IPublisher();
			virtual ~IPublisher() {}
			virtual void Unsubscribe(Subscriber* subscriber) = 0;

		protected:
			bool m_publishing;
			bool m_erasure;
		};

		//templated publisher
		template<typename TYPE>
		class Publisher : public IPublisher
		{
		public:
			typedef std::function<void(TYPE)> Callback;
			typedef std::pair<Subscriber*, Callback> Subscription;
			typedef typename std::list<Subscription>::iterator SubIter;

			void Publish(TYPE arg);
			void Subscribe(Subscriber* subscriber, Callback callback);
			virtual void Unsubscribe(Subscriber* subscriber);

		protected:
			std::list<Subscription> m_subscriptions;
			std::vector<Subscription> m_pending;
			SubIter m_iter;
		};

		

		//explicit void publisher
		class VoidPublisher : public IPublisher
		{
			typedef std::function<void(void)> VoidCallback;
			typedef std::pair<Subscriber*, VoidCallback> Subscription;
			typedef std::list<Subscription>::iterator SubIter;

		public:
			void Publish();
			void Subscribe(Subscriber* subscriber, VoidCallback callback);
			virtual void Unsubscribe(Subscriber* subscriber);

		protected:
			std::list<Subscription> m_subscriptions;
			std::vector<Subscription> m_pending;
			SubIter m_iter;
		};

		//convenience types
		typedef Publisher<const bool&>						BoolPublisher;
		typedef Publisher<const int&>						IntPublisher;
		typedef Publisher<const float&>						FloatPublisher;
		typedef Publisher<const TBuffer&>					BufferPublisher;
		typedef Publisher<const util::Time&>				TimePublisher;
		typedef Publisher<const std::string&>				StringPublisher;
		typedef Publisher<const util::math::vec2&>			Vec2Publisher;
		typedef Publisher<const util::math::vec3&>			Vec3Publisher;
		typedef Publisher<const util::shape::AABB&>			AabbPublisher;
		typedef Publisher<const util::math::Matrix2D&>		MatrixPublisher;

	}
}