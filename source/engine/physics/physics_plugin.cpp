#include "physics_plugin.h"
#include "func.h"
#include "../render/opengl.h"
#include "../render/render_plugin.h"

namespace engine
{
	namespace physics
	{
		class QueryCallback : public b2QueryCallback
		{
		public:
			QueryCallback(const b2Vec2& point)
			{
				m_point = point;
				m_fixture = NULL;
			}

			bool ReportFixture(b2Fixture* fixture)
			{
				b2Body* body = fixture->GetBody();
				if (body->GetType() == b2_dynamicBody)
				{
					bool inside = fixture->TestPoint(m_point);
					if (inside)
					{
						m_fixture = fixture;

						// We are done, terminate the query.
						return false;
					}
				}

				// Continue the query.
				return true;
			}

			b2Vec2 m_point;
			b2Fixture* m_fixture;
		};

		const int VELOCITY_ITERATIONS = 8;
		const int POSITION_ITERATIONS = 3;
		const b2Vec2 GRAVITY = b2Vec2(0.0f, -10.0f);


		DEFINE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		PhysicsPlugin::PhysicsPlugin()
			: m_pWorld(null)
			, m_pMouseJoint(null)
			, m_pGroundBody(null)
			, m_bRightMouseBtn(false)
			, m_bLeftMouseBtn(false)
			, m_state(LARK)
			, m_pRopeJoint(null)
		{
		}

		VIRTUAL PhysicsPlugin::~PhysicsPlugin()
		{
		}

		VIRTUAL void PhysicsPlugin::Init()
		{
			input::mouse_events::s_InputMouseButtonDown.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseDown));
			input::mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseUp));
			input::mouse_events::s_InputMouseMotion.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseMotion));
			input::mouse_events::s_InputMouseScrollWheel.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnMouseWheel));
			input::key_events::s_InputKeyDown.Subscribe(&m_sub, BIND1(this, &PhysicsPlugin::OnKeyDown));

			m_pWorld = new b2World(GRAVITY);
			m_pWorld->SetAllowSleeping(true);
			m_pWorld->SetWarmStarting(true);
			m_pWorld->SetContinuousPhysics(true);
			m_pWorld->SetSubStepping(false);

			m_debugDraw.Create();
			m_pWorld->SetDebugDraw(&m_debugDraw);
			uint32 flags = 0;
			flags += true * b2Draw::e_shapeBit;
			flags += true * b2Draw::e_jointBit;
			flags += false * b2Draw::e_aabbBit;
			flags += false * b2Draw::e_centerOfMassBit;
			m_debugDraw.SetFlags(flags);

			//creat shape (test)
			b2BodyDef bd;
			b2Body* ground = m_pWorld->CreateBody(&bd);
			b2EdgeShape shapeG;
			shapeG.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shapeG, 0.0f);


			b2BodyDef bodyDef;
			m_pGroundBody = m_pWorld->CreateBody(&bodyDef);

			b2CircleShape shapec;
			shapec.m_radius = 1.0f;
			b2BodyDef bdc;
			bdc.type = b2_dynamicBody;
			bdc.position.Set(0.0, 20.0f);
			m_pLark = m_pWorld->CreateBody(&bdc);
			m_pLark->CreateFixture(&shapec, 10.0f);


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

			for (int32 i = 0; i < e_count; ++i)
			{
				y = x;
				
				for (int32 j = i; j < e_count; ++j)
				{
					if (i % 5 == 0)
					{
						b2BodyDef bd;
						bd.type = b2_staticBody;
						bd.position = y;
						b2Body* body = m_pWorld->CreateBody(&bd);
						body->CreateFixture(&shape, 5.0f);
					}

					y += deltaY;
				}

				x += deltaX;
			}
		}

		VIRTUAL void PhysicsPlugin::Exit()
		{
			m_sub.UnsubscribeAll();
			m_debugDraw.Destroy();
			SafeDelete(m_pWorld);
		}

		VIRTUAL bool PhysicsPlugin::Update(const util::Time& dt)
		{
			if (m_state == LARK)
			{
				g_camera.m_center = m_pLark->GetPosition();
				//apply forces
				if (m_bLeftMouseBtn)
				{
					b2Vec2 diff = m_pLark->GetPosition() - lastp;
					diff.Normalize();
					diff *= 1000.0f;

					m_pLark->ApplyForceToCenter(diff, true);
				}
			}

			double delta = dt.ToDouble() / 1000;
			m_pWorld->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			m_pWorld->DrawDebugData();

			if (m_pMouseJoint)
			{
				b2Vec2 p1 = m_pMouseJoint->GetAnchorB();
				b2Vec2 p2 = m_pMouseJoint->GetTarget();

				b2Color c;
				c.Set(0.0f, 1.0f, 0.0f);
				m_debugDraw.DrawPoint(p1, 4.0f, c);
				m_debugDraw.DrawPoint(p2, 4.0f, c);

				c.Set(0.8f, 0.8f, 0.8f);
				m_debugDraw.DrawSegment(p1, p2, c);
			}

			m_debugDraw.Flush();

			engine::render::RenderPlugin* plug = static_cast<engine::render::RenderPlugin*>(engine::IPlugin::FindPlugin(engine::render::RenderPlugin::Type));
			SDL_GL_SwapWindow(plug->GetSdlWindow());

			return true;
		}

		void PhysicsPlugin::OnKeyDown(const input::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case SDLK_l: m_state = LARK; break;
			case SDLK_t: m_state = TESTBED; break;
			case SDLK_SPACE:
			{
				if (m_state == LARK)
					m_state = TESTBED;
				else
					m_state = LARK;
				break;
			}
			}
		}

		void PhysicsPlugin::OnMouseDown(const input::mouse_events::ButtonAction& action)
		{
			switch (m_state)
			{
			case TESTBED:
				OnMouseDownTestbed(action);
				break;
			case LARK:
				OnMouseDownLark(action);
				break;
			}
		}

		void PhysicsPlugin::OnMouseUp(const input::mouse_events::ButtonAction& action)
		{
			switch (m_state)
			{
			case TESTBED:
				OnMouseUpTestbed(action);
				break;
			case LARK:
				OnMouseUpLark(action);
				break;
			}
		}

		void PhysicsPlugin::OnMouseMotion(const input::mouse_events::MotionAction& action)
		{
			switch (m_state)
			{
			case TESTBED:
				OnMouseMotionTestbed(action);
				break;
			case LARK:
				OnMouseMotionLark(action);
				break;
			}
		}

		void PhysicsPlugin::OnMouseWheel(const input::mouse_events::WheelAction& action)
		{
			if (m_state != TESTBED)
				return;

			if (action.m_scroll.y > 0)
			{
				g_camera.m_zoom /= 1.1f;
			}
			else
			{
				g_camera.m_zoom *= 1.1f;
			}
		}


		void PhysicsPlugin::OnMouseDownTestbed(const input::mouse_events::ButtonAction& action)
		{
			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (action.m_button == SDL_BUTTON_RIGHT && !m_bRightMouseBtn)
			{
				m_bRightMouseBtn = true;
				lastp = pw;
				return;
			}
			else if (action.m_button == SDL_BUTTON_LEFT)
			{

				if (m_pMouseJoint != NULL)
				{
					return;
				}

				// Make a small box.
				b2AABB aabb;
				b2Vec2 d;
				d.Set(0.001f, 0.001f);
				aabb.lowerBound = pw - d;
				aabb.upperBound = pw + d;

				// Query the world for overlapping shapes.
				QueryCallback callback(pw);
				m_pWorld->QueryAABB(&callback, aabb);

				if (callback.m_fixture)
				{
					b2Body* body = callback.m_fixture->GetBody();
					b2MouseJointDef md;
					md.bodyA = m_pGroundBody;
					md.bodyB = body;
					md.target = pw;
					md.maxForce = 1000.0f * body->GetMass();
					m_pMouseJoint = (b2MouseJoint*)m_pWorld->CreateJoint(&md);
					body->SetAwake(true);
				}
			}
		}
		void PhysicsPlugin::OnMouseUpTestbed(const input::mouse_events::ButtonAction& action)
		{
			if (action.m_button == SDL_BUTTON_RIGHT)
			{
				m_bRightMouseBtn = false;
				return;
			}

			if (m_pMouseJoint)
			{
				m_pWorld->DestroyJoint(m_pMouseJoint);
				m_pMouseJoint = NULL;
			}
		}
		void PhysicsPlugin::OnMouseMotionTestbed(const input::mouse_events::MotionAction& action)
		{
			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (m_bRightMouseBtn)
			{
				b2Vec2 diff = pw - lastp;
				g_camera.m_center.x -= diff.x;
				g_camera.m_center.y -= diff.y;
				lastp = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));
				return;
			}

			if (m_pMouseJoint)
			{
				m_pMouseJoint->SetTarget(pw);
			}
		}


		void PhysicsPlugin::OnMouseDownLark(const input::mouse_events::ButtonAction& action)
		{
			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (action.m_button == SDL_BUTTON_LEFT && !m_bLeftMouseBtn)
			{
				m_bLeftMouseBtn = true;
				lastp = pw;
				return;
			}
			
		}
		void PhysicsPlugin::OnMouseUpLark(const input::mouse_events::ButtonAction& action)
		{
			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (action.m_button == SDL_BUTTON_LEFT)
			{
				m_bLeftMouseBtn = false;
			}
			else if (action.m_button == SDL_BUTTON_RIGHT && m_pRopeJoint == null)
			{
				b2Body* body = m_pLark;
				b2RopeJointDef md;
				md.bodyA = m_pGroundBody;
				md.bodyB = body;
				md.localAnchorA = pw;
				md.localAnchorB.SetZero();
				md.maxLength = (pw - m_pLark->GetPosition()).Length() * 1.1f, 20.0f;
				m_pRopeJoint = (b2RopeJoint*)m_pWorld->CreateJoint(&md);
				body->SetAwake(true);
			}
			else if (action.m_button == SDL_BUTTON_RIGHT)
			{
				m_pWorld->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
		}
		void PhysicsPlugin::OnMouseMotionLark(const input::mouse_events::MotionAction& action)
		{
			b2Vec2 pw = g_camera.ConvertScreenToWorld(b2Vec2(action.m_pixel.x, action.m_pixel.y));

			if (m_bLeftMouseBtn)
			{
				lastp = pw;
			}
		}

	}
}