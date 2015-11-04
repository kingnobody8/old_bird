#include "camera.h"
#include "func.h"

namespace engine
{
	namespace render
	{
		STATIC std::list<CCamera*> CCamera::s_cameras;

		STATIC CCamera* CCamera::FindCamera(const std::string& name)
		{
			for (auto iter = s_cameras.begin(); iter != s_cameras.end(); ++iter)
			{
				if ((*iter)->GetName() == name)
				{
					return (*iter);
				}
			}
			return null;
		}
		STATIC CCamera* CCamera::CreateCamera(const std::string& name)
		{
			assert(!name.empty());

			CCamera* cam = new CCamera(name);
			s_cameras.push_back(cam);
			return cam;
		}
		STATIC void CCamera::DestroyCameras()
		{
			for (auto iter = s_cameras.begin(); iter != s_cameras.end(); ++iter)
				delete (*iter);
			s_cameras.clear();
		}

		CCamera::CCamera(const std::string& name)
			: m_name(name)
		{

		}

		CCamera::~CCamera()
		{
		}

		const util::shape::AABB CCamera::CalcViewAabb(const util::math::vec2& half_screen_dims) const
		{
			//create an aabb with the screen dims
			util::shape::AABB ret(half_screen_dims * -1, half_screen_dims);
		
			//modify the aabb by our inverse matrix
			util::math::Matrix2D inverse = util::math::Matrix2D::Matrix_Inverse(m_matrix);
			ret.m_min = util::math::Matrix2D::Vector_Matrix_Multiply(ret.m_min, inverse);
			ret.m_max = util::math::Matrix2D::Vector_Matrix_Multiply(ret.m_max, inverse);
			
			return ret;
		}
	}
}