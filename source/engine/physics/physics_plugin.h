#pragma once
#include "plugin.h"
#include "box2d/Box2D.h"
#include "debug_draw.h"

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

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

		};
	}
}