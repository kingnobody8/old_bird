#pragma once
#include "../input/input_event.h"
#include "box2d/Box2D.h"


namespace engine
{
	

	class LarkController
	{
	public:
		LarkController();
		~LarkController();

		void Init(b2World* pWorld, b2Body* pLarkBody);
		void Update(const util::Time& dt);
		void Exit();

		b2Body* GetLarkBody() {	return m_pLarkBody; }
		void SetTestbed(const bool b) {
			m_bIsTestbed = b;
		}

	private:
		void ActivatePush();
		void DeactivatePush();
		void OnRopeEvent(const vec2& screenPos);

#ifdef IS_MOBILE
		void OnTouchDown(const touch_event::TouchAction& action);
		void OnTouchUp(const touch_event::TouchAction& action);
		void OnTouchMotion(const touch_event::MotionAction& action);

		void PushCheck(const int& fingerId);

		enum EFingerDefs	{ eInvalidFinger = -1, eFirstFinger = 1, eSecondFinger, eThirdFinger, eFingerCount };
		enum EFingerTypes	{ eInvalidType, eUnknown, ePush, eRope };

		struct FingerInfo
		{
			util::Time m_time = 0;
			vec2 m_position;
			EFingerTypes m_type = EFingerTypes::eInvalidType;
			bool m_bDown = false;
		};
#elif IS_PC
		void OnMouseDown(const mouse_events::ButtonAction& action);
		void OnMouseUp(const mouse_events::ButtonAction& action);
		void OnMouseMotion(const mouse_events::MotionAction& action);

		enum EMouseDefs { eInvalidButton = -1, eLeftButton, eRightButton, eButtonCount };
		struct MouseInfo
		{
			vec2 m_position;
			bool m_bDown = false;
		};
#endif

	private:
		b2World* m_pWorld;
		b2Body* m_pLarkBody;
		b2Joint* m_pRopeJoint;
#ifdef IS_MOBILE
		FingerInfo m_vFingerInfo[EFingerDefs::eFingerCount];
#elif IS_PC
		MouseInfo m_vMouseInfo[EMouseDefs::eButtonCount];
#endif
		bool m_bIsPushActive;
		bool m_bIsTestbed;

		util::Subscriber m_sub;
	};
}