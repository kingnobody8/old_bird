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

		const b2AABB CCamera::CalcViewAabb(const util::math::vec2& half_screen_dims) const
		{
			//create an aabb with the screen dims
			b2AABB ret;
			ret.lowerBound = b2Vec2(-half_screen_dims.x, -half_screen_dims.y);
			ret.upperBound = b2Vec2(half_screen_dims.y, half_screen_dims.y);
		
			//modify the aabb by our inverse matrix
			util::math::Matrix2D inverse = util::math::Matrix2D::Matrix_Inverse(m_matrix);
			util::math::vec2 min = util::math::Matrix2D::Vector_Matrix_Multiply(util::math::vec2(ret.lowerBound.x, ret.lowerBound.y), inverse);
			util::math::vec2 max = util::math::Matrix2D::Vector_Matrix_Multiply(util::math::vec2(ret.upperBound.x, ret.upperBound.y), inverse);
			
			ret.lowerBound = b2Vec2(Min(min.x, max.x), Min(min.y, max.y));
			ret.upperBound = b2Vec2(Max(min.x, max.x), Max(min.y, max.y));

			return ret;
		}
	}
}