#include "physics_plugin.h"

namespace engine
{
	namespace physics
	{
		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
		{
		}

		VIRTUAL PhysicsPlugin::~PhysicsPlugin()
		{
		}

		VIRTUAL void PhysicsPlugin::Init()
		{
			
		}

		VIRTUAL void PhysicsPlugin::Exit()
		{
		}

		VIRTUAL bool PhysicsPlugin::Update(const util::Time& dt)
		{
			return true;
		}

	}
}