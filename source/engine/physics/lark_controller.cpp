#include "lark_controller.h"
#include "physics/physics_plugin.h"
#include "callbacks/raycast_callback.h"

#define ROPE_TIME 500

namespace engine
{
	LarkController::LarkController()
		: m_pLarkBody(NULL)
		, m_bIsPushActive(false)
		, m_pWorld(NULL)
		, m_pRopeJoint(NULL)
		, m_bIsTestbed(false)
	{
	}

	LarkController::~LarkController()
	{
	}

	void LarkController::Init(b2World* pWorld, b2Body* pLarkBody)
	{
		m_pWorld = pWorld;
		m_pLarkBody = pLarkBody;

		mouse_events::s_InputMouseButtonDown.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseDown));
		mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseUp));
		mouse_events::s_InputMouseMotion.Subscribe(&m_sub, BIND1(this, &LarkController::OnMouseMotion));
	}

	void LarkController::Update(const util::Time& dt)
	{
		if (!m_bIsPushActive)
			return;
	
		vec2 screenPos;

#ifdef IS_MOBILE
		//average the two finger positions
		vec2 averageScreenPosition;
		for (int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			if (m_vFingerInfo[i].m_type != EFingerTypes::ePush)
			{
				continue;
			}
			averageScreenPosition += m_vFingerInfo[i].m_position;
		}
		averageScreenPosition /= 2.0f; //there should always be 2 fingers for push
		screenPos = averageScreenPosition;
#elif IS_PC
		screenPos = m_vMouseInfo[eLeftButton].m_position;
#endif
		b2Vec2 pw = physics::g_camera.ConvertScreenToWorld(b2Vec2(screenPos.x, screenPos.y));
		b2Vec2 diff = m_pLarkBody->GetPosition() - pw;
		diff.Normalize();
		diff *= 1000.0f;
		m_pLarkBody->ApplyForceToCenter(diff, true);
	}

	void LarkController::Exit()
	{
	}


	void LarkController::OnMouseDown(const mouse_events::ButtonAction& action)
	{
		if(m_bIsTestbed)
			return;

#ifdef IS_MOBILE
		int fingerId = action.m_event.tfinger.fingerId;
		//don't use more than 3 fingers
		if (fingerId > EFingerDefs::eFingerCount)
			return;

		//set values
		FingerInfo& info = m_vFingerInfo[fingerId];
		info.m_bDown = true;
		info.m_time = 0;
		info.m_position = action.m_pixel;
		info.m_type = EFingerTypes::eUnknown;

		//push check
		PushCheck(fingerId);
#elif IS_PC

		if (action.m_button == SDL_BUTTON_LEFT)
		{
			m_vMouseInfo[eLeftButton].m_bDown = true;
			m_vMouseInfo[eLeftButton].m_position = action.m_pixel;
			ActivatePush();
		}
		else if (action.m_button == SDL_BUTTON_RIGHT)
		{
			m_vMouseInfo[eRightButton].m_bDown = true;
			m_vMouseInfo[eRightButton].m_position = action.m_pixel;
		}
#endif
	}

	void LarkController::OnMouseUp(const mouse_events::ButtonAction& action)
	{
		if(m_bIsTestbed)
			return;

#ifdef IS_MOBILE
		int fingerId = action.m_event.tfinger.fingerId;
		//don't use more than 3 fingers
		if (fingerId > EFingerDefs::eFingerCount)
			return;

		//set values (pt1)
		FingerInfo& info = m_vFingerInfo[fingerId];
		info.m_position = action.m_pixel;
		info.m_bDown = false;

		switch (info.m_type)
		{
		case EFingerTypes::eRope:
		case EFingerTypes::eUnknown:	//Unkown and rope could both activate a rope if the timing is right
		{
			if (info.m_time.Milli() < ROPE_TIME)
			{
				OnRopeEvent(info.m_position);
			}
			break;
		}
		case EFingerTypes::ePush:
		{
			DeactivatePush();
			break;
		}
		}

		//set values (pt2)
		info.m_time = 0;
		info.m_type = EFingerTypes::eInvalidType;
#elif IS_PC

		if (action.m_button == SDL_BUTTON_LEFT)
		{
			m_vMouseInfo[eLeftButton].m_bDown = false;
			m_vMouseInfo[eLeftButton].m_position = action.m_pixel;
			DeactivatePush();
		}
		else if (action.m_button == SDL_BUTTON_RIGHT)
		{
			m_vMouseInfo[eRightButton].m_bDown = false;
			m_vMouseInfo[eRightButton].m_position = action.m_pixel;
			OnRopeEvent(action.m_pixel);
		}
#endif
	}

	void LarkController::OnMouseMotion(const mouse_events::MotionAction& action)
	{
		if(m_bIsTestbed)
			return;

#ifdef IS_MOBILE
		int fingerId = action.m_event.tfinger.fingerId;
		//don't use more than 3 fingers
		if (fingerId > EFingerDefs::eFingerCount)
			return;

		//set values
		FingerInfo& info = m_vFingerInfo[fingerId];
		info.m_position = action.m_pixel;
#elif IS_PC
		for (int i = 0; i < EMouseDefs::eButtonCount; ++i)
		{
			m_vMouseInfo[i].m_position = action.m_pixel;
		}
#endif
	}

#ifdef IS_MOBILE
	void LarkController::PushCheck(const int& fingerId)
	{
		FingerInfo& info = m_vFingerInfo[fingerId];
		assert(!info.m_bDown && info.m_type != EFingerTypes::eUnknown);

		//find another finger we can use
		int otherFingerId = -1;
		for (int i = 0; i < EFingerDefs::eFingerCount; ++i)
		{
			if (i == fingerId)
			{
				continue;
			}
			if (m_vFingerInfo[i].m_bDown && m_vFingerInfo[i].m_type == EFingerTypes::eUnknown)
			{
				otherFingerId = i;
				break;
			}
		}

		if (otherFingerId != -1)
		{
			FingerInfo& otherInfo = m_vFingerInfo[otherFingerId];

			info.m_type = EFingerTypes::ePush;
			otherInfo.m_type = EFingerTypes::ePush;

			ActivatePush();
		}
	}
#endif

	void LarkController::ActivatePush()
	{
		m_bIsPushActive = true;
	}

	void LarkController::DeactivatePush()
	{
		m_bIsPushActive = false;
	}

	void LarkController::OnRopeEvent(const vec2& screenPos)
	{
		if (m_pRopeJoint == NULL)
		{
			b2Vec2 pw = physics::g_camera.ConvertScreenToWorld(b2Vec2(screenPos.x, screenPos.y));
			physics::callbacks::RayCastClosestCallback callback;
			b2Vec2 direction = pw - m_pLarkBody->GetPosition();
			direction.Normalize();
			direction *= 100;
			b2Vec2 point = m_pLarkBody->GetPosition() + direction;
			m_pWorld->RayCast(&callback, m_pLarkBody->GetPosition(), point);

			if (callback.m_bHit)
			{
				b2Body* body = callback.m_pFixture->GetBody();
				b2RopeJointDef md;
				md.bodyA = m_pLarkBody;
				md.bodyB = body;
				md.localAnchorA.SetZero();
				md.localAnchorB = body->GetLocalPoint(callback.m_point);
				md.maxLength = (callback.m_point - m_pLarkBody->GetPosition()).Length() * 1.1f;
				md.collideConnected = true;
				m_pRopeJoint = (b2RopeJoint*)m_pWorld->CreateJoint(&md);
				body->SetAwake(true);
			}
		}
		else
		{
			m_pWorld->DestroyJoint(m_pRopeJoint);
			m_pRopeJoint = NULL;
		}
	}
}