#include "box2d_part.h"
#include "script/script.h"

namespace engine
{
	namespace script
	{
		const int VELOCITY_ITERATIONS = 8;
		const int POSITION_ITERATIONS = 3;

		STATIC const float CBox2DPart::PIX_TO_BOX = 0.015625f;
		STATIC const float CBox2DPart::BOX_TO_PIX = 64.0f;

		STATIC b2World* CBox2DPart::s_pWorld = null;

		STATIC void CBox2DPart::SetupWorld()
		{
			b2Vec2 gravity;
			gravity.Set(0.0f, -10.0f);
			s_pWorld = new b2World(gravity);
		}

		STATIC void CBox2DPart::DestroyWorld()
		{
			__todo() //remove all the box2d parts here???
			SafeDelete(s_pWorld);
		}

		STATIC void CBox2DPart::UpdateWorld(const util::Time& dt)
		{
			s_pWorld->SetAllowSleeping(true);
			s_pWorld->SetWarmStarting(true);
			s_pWorld->SetContinuousPhysics(true);
			s_pWorld->SetSubStepping(false);

			double delta = dt.ToDouble() / 1000;
			s_pWorld->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		}

		CBox2DPart::CBox2DPart()
			: m_bSettingMatrix(false)
		{
		}
		VIRTUAL CBox2DPart::~CBox2DPart()
		{
		}
		CBox2DPart& CBox2DPart::operator = (const CBox2DPart& that)
		{
			return *this;
		}

		VIRTUAL void CBox2DPart::Init()
		{
		}

		VIRTUAL void CBox2DPart::LoadJson(const util::JSON& json)
		{
			IRenderPart::LoadJson(json);
		}

		VIRTUAL void CBox2DPart::OnMatrixChanged(void)
		{
			if (m_bSettingMatrix) //we only want to respond to a matrix changed event, if someone else changes our matrix
				return;
		}

		

	}
}