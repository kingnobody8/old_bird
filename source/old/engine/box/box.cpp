#include "engine/box/box.h"
#include <box2d/Box2D.h>

namespace Engine
{
	namespace Box
	{
		CBox* CBox::instance = nullptr;
		CBox* CBox::Get(void)
		{
			if (!instance)
			{
				instance = new CBox();
				b2Vec2 gravity;
				gravity.Set(0.0f, -10.0f);
				instance->m_pWorld = new b2World(gravity);
			}
			return instance;
		}
		void CBox::DeleteInstance(void)
		{
			if (instance)
			{//TODO destroy the world here

				delete instance->m_pWorld;
				delete instance;
				instance = nullptr;
			}
		}

		CBox::CBox(void)
			: m_pWorld(null)
		{
		}
		CBox::~CBox(void)
		{
		}

		void CBox::Update(Util::Time dt)
		{
			m_pWorld->SetAllowSleeping(true);
			m_pWorld->SetWarmStarting(true);
			m_pWorld->SetContinuousPhysics(true);
			m_pWorld->SetSubStepping(false);

			m_pWorld->Step(dt, 8, 3);
		}
	}
}