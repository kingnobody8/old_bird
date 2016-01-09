#pragma once
#include "plugin.h"
#include "box2d/Box2D.h"
#include "debug_draw.h"
#include "../input/input_event.h"

namespace engine
{
	namespace physics
	{
		class PhysicsPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		private:
			b2World* m_pWorld;
			DebugDraw m_debugDraw;
			util::event::Subscriber m_sub;
			b2MouseJoint* m_pMouseJoint;
			b2Body* m_pGroundBody;
			bool m_bRightMouseBtn;
			b2Vec2 lastp;


			void OnMouseDown(const input::mouse_events::ButtonAction& action);
			void OnMouseUp(const input::mouse_events::ButtonAction& action);
			void OnMouseMotion(const input::mouse_events::MotionAction& action);
			void OnMouseWheel(const input::mouse_events::WheelAction& action);

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

		};
	}
}