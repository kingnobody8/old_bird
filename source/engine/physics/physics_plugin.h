#pragma once
#include "plugin.h"
#include "box2d/Box2D.h"
#include "debug_draw.h"
#include "../input/input_event.h"
#include "../b2djson/b2dJson.h"

namespace engine
{
	namespace physics
	{

		// This callback finds the closest hit. Polygon 0 is filtered.
		class RayCastClosestCallback : public b2RayCastCallback
		{
		public:
			RayCastClosestCallback()
			{
				m_pFixture = null;
				m_hit = false;
			}

			float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
			{
				b2Body* body = fixture->GetBody();
				void* userData = body->GetUserData();
				if (userData)
				{
					int32 index = *(int32*)userData;
					if (index == 0)
					{
						// By returning -1, we instruct the calling code to ignore this fixture and
						// continue the ray-cast to the next fixture.
						return -1.0f;
					}
				}

				m_hit = true;
				m_point = point;
				m_normal = normal;
				m_pFixture = fixture;

				// By returning the current fraction, we instruct the calling code to clip the ray and
				// continue the ray-cast to the next fixture. WARNING: do not assume that fixtures
				// are reported in order. However, by clipping, we can always get the closest fixture.
				return fraction;
			}

			bool m_hit;
			b2Vec2 m_point;
			b2Vec2 m_normal;
			b2Fixture* m_pFixture;
		};


		class PhysicsPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(PhysicsPlugin);

			enum EInputState { TESTBED, LARK, };

		private:
			b2World* m_pWorld;
			DebugDraw m_debugDraw;
			util::event::Subscriber m_sub;
			b2MouseJoint* m_pMouseJoint;
			b2RopeJoint* m_pRopeJoint;
			b2Body* m_pGroundBody;
			b2Body* m_pLark;
			b2Body* m_pAttach;
			bool m_bLeftMouseBtn;
			bool m_bRightMouseBtn;
			b2Vec2 lastp;
			EInputState m_state;
			b2dJson b2d;
			b2ParticleSystem* m_pParticleSystem;

			void OnMouseDownTestbed(const input::mouse_events::ButtonAction& action);
			void OnMouseUpTestbed(const input::mouse_events::ButtonAction& action);
			void OnMouseMotionTestbed(const input::mouse_events::MotionAction& action);

			void OnMouseDownLark(const input::mouse_events::ButtonAction& action);
			void OnMouseUpLark(const input::mouse_events::ButtonAction& action);
			void OnMouseMotionLark(const input::mouse_events::MotionAction& action);

			void OnMouseDown(const input::mouse_events::ButtonAction& action);
			void OnMouseUp(const input::mouse_events::ButtonAction& action);
			void OnMouseMotion(const input::mouse_events::MotionAction& action);
			void OnMouseWheel(const input::mouse_events::WheelAction& action);

			void OnKeyDown(const input::key_events::KeyAction& action);

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

		};
	}
}