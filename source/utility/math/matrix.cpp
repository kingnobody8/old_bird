#include "matrix.h"

const int CR_POS_X = 1;
const int CR_POS_Y = 1;
const int CR_POS_Z = 1;

namespace util
{
	namespace math
	{

		Matrix& Matrix::operator = (const glm::f32mat4x4& that)
		{
			glm::f32mat4x4::operator=(that);
			return *this;
		}

		const glm::f32vec3 Matrix::GetPosition() const
		{
			glm::f32vec3 ret;
			return ret;
		}
		const glm::f32vec3 Matrix::GetRotation() const
		{
			glm::f32vec3 ret;
			return ret;
		}
		const glm::f32vec3 Matrix::GetScale() const
		{
			glm::f32vec3 ret;
			return ret;
		}

		const void  Matrix::SetPosition(const glm::f32vec3& pos)
		{

		}
		const void  Matrix::SetRotation(const glm::f32vec3& rot)
		{

		}
		const void  Matrix::SetScale(const glm::f32vec3& scale)
		{

		}

		const void  Matrix::SetPosition(const glm::f32& x, const glm::f32& y, const glm::f32& z)
		{
		}
		const void  Matrix::SetRotation(const glm::f32& x, const glm::f32& y, const glm::f32& z)
		{
		}
		const void  Matrix::SetScale(const glm::f32& x, const glm::f32& y, const glm::f32& z)
		{

		}

		const void  Matrix::SetPositionX(const glm::f32& x)
		{
		}
		const void  Matrix::SetPositionY(const glm::f32& y)
		{
		}
		const void  Matrix::SetPositionZ(const glm::f32& z)
		{
		}
	}
}