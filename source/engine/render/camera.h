#pragma once
#include "math/matrix_2d.h"
#include <list>
#include "box2d//Box2D.h"
#include <string>

namespace engine
{
	namespace render
	{
		class CCamera
		{
		private:
			static std::list<CCamera*> s_cameras;

		public:
			static CCamera* FindCamera(const std::string& name);
			static CCamera* CreateCamera(const std::string& name);
			static void DestroyCameras();

		private:
			std::string m_name;
			util::math::Matrix2D m_matrix;

		public:
			CCamera(const std::string& name); __todo() // i belive this method of creating an object (as well as in the render layers) is called Factory method, i should study this and make sure i am doing it correctly. i am pretty sure this is a way to prevent the class from being instantiated outside of the create function
			~CCamera();

			inline void SetMatrix(const util::math::Matrix2D& mat) { m_matrix = mat; }
			inline const util::math::Matrix2D& GetMatrix() const { return m_matrix; }
			inline const std::string& GetName() const { return m_name; }
			const b2AABB CalcViewAabb(const util::math::vec2& half_screen_dims) const;
		};
	}
}