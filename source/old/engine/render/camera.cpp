#include "engine/render/camera.h"
#include "engine/component/object.h"

namespace Engine
{
	namespace Render
	{
		CCamera::CCamera(vec2 dimensions)
			: m_pFollow(nullptr)
		{

		}

		void CCamera::Update(const Util::Time& dt)
		{
			if (m_pFollow)
			{
				this->m_cMat.SetPosition(m_pFollow->CalcWorldMatrix().GetPosition());
			}
		}

		EVENT_CALLBACK(CCamera, OnCameraFollow, CCameraFollow)
		{
			this->m_pFollow = event.pObj;
			return false;
		}

	}
}