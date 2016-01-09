#include "physics_plugin.h"
#include "func.h"
#include "../render/opengl.h"
#include "../render/render_plugin.h"

namespace engine
{
	namespace physics
	{
		const int VELOCITY_ITERATIONS = 8;
		const int POSITION_ITERATIONS = 3;
		const b2Vec2 GRAVITY = b2Vec2(0.0f, -10.0f);


		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
			: m_pWorld(null)
		{
		}

		VIRTUAL PhysicsPlugin::~PhysicsPlugin()
		{
		}

		VIRTUAL void PhysicsPlugin::Init()
		{
			m_pWorld = new b2World(GRAVITY);
			m_pWorld->SetAllowSleeping(true);
			m_pWorld->SetWarmStarting(true);
			m_pWorld->SetContinuousPhysics(true);
			m_pWorld->SetSubStepping(false);

			m_debugDraw.Create();
			m_pWorld->SetDebugDraw(&m_debugDraw);
			uint32 flags = 0;
			flags += true			* b2Draw::e_shapeBit;
			flags += true			* b2Draw::e_jointBit;
			flags += false			* b2Draw::e_aabbBit;
			flags += false				* b2Draw::e_centerOfMassBit;
			m_debugDraw.SetFlags(flags);

			//creat shape (test)
			b2BodyDef bd;
			b2Body* ground = m_pWorld->CreateBody(&bd);
			b2EdgeShape shapeG;
			shapeG.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shapeG, 0.0f);

			float32 a = 0.5f;
			b2PolygonShape shape;
			shape.SetAsBox(a, a);

			b2Vec2 x(-7.0f, 0.75f);
			b2Vec2 y;
			b2Vec2 deltaX(0.5625f, 1.25f);
			b2Vec2 deltaY(1.125f, 0.0f);
			int e_count = 20;

			for (int32 i = 0; i < e_count; ++i)
			{
				y = x;

				for (int32 j = i; j < e_count; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					bd.position = y;
					b2Body* body = m_pWorld->CreateBody(&bd);
					body->CreateFixture(&shape, 5.0f);

					y += deltaY;
				}

				x += deltaX;
			}
		}

		VIRTUAL void PhysicsPlugin::Exit()
		{
			m_debugDraw.Destroy();
			SafeDelete(m_pWorld);
		}

		VIRTUAL bool PhysicsPlugin::Update(const util::Time& dt)
		{
			double delta = dt.ToDouble() / 1000;
			m_pWorld->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			m_pWorld->DrawDebugData();
			m_debugDraw.Flush();

			engine::render::RenderPlugin* plug = static_cast<engine::render::RenderPlugin*>(engine::IPlugin::FindPlugin(engine::render::RenderPlugin::Type));
			SDL_GL_SwapWindow(plug->GetSdlWindow());

			return true;
		}

	}
}