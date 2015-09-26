#pragma once
#include "engine/asdf.h"
#include "engine/event/event.h"
#include "engine/math/math.h"

using namespace Engine::Math;

namespace Engine
{
	namespace Render
	{
		struct CCameraFollow : public Event::IEvent
		{
			Object::CObject* pObj;
		};

		class CCamera
		{
		private:
			CMatrix2D	m_cMat;
			CRect		m_cRect;
			Object::CObject* m_pFollow;

		private:
			PROTO_EVENT_CALLBACK(OnCameraFollow, CCameraFollow);

		public:
			CCamera(vec2 dimensions);
			void Update(const Util::Time& dt);

			//Gets
			inline const CMatrix2D&	GetMatrix(void) const { return this->m_cMat; }
			//TODO When i eventually use this for camera culling, I will need to do something so that the rect is rotated if the camera matrix is
			inline const CRect&		GetRect(void) const { return this->m_cRect; }
			inline Object::CObject* const GetFollow(void) const { return m_pFollow; }

			//Sets
			inline void SetMatrix(const CMatrix2D& mat) { this->m_cMat = mat; }
			inline void SetFollow(Engine::Object::CObject* const pObj) { this->m_pFollow = pObj; }
		};
	}
}