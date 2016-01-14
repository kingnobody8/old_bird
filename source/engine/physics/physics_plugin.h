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
		class PhysicsPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(PhysicsPlugin);

			enum EInputState { TESTBED, LARK, };

		private:
			b2dJson m_json;
			b2World* m_pWorld;
			DebugDraw m_debugDraw;
			util::event::Subscriber m_sub;
			b2MouseJoint* m_pMouseJoint;
			b2RopeJoint* m_pRopeJoint;
			b2Body* m_pGroundBody;
			b2Body* m_pLark;
			bool m_bLeftMouseBtn;
			bool m_bRightMouseBtn;
			b2Vec2 lastp;
			EInputState m_state;
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