#include "transform.h"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"
namespace util
{
	Transform::Transform()
		: m_dirty(true)
		, m_scale(1.0f, 1.0f, 1.0f)
	{
	}

	Transform::Transform(const vec3& pos, const vec3& rot, const vec3& scale)
		: m_dirty(true)
		, m_pos(pos)
		, m_rotation(rot)
		, m_scale(scale)
	{
	}

	const void  Transform::SetPosition(const glm::f32vec3& pos)
	{

	}
	const void  Transform::SetRotation(const glm::f32vec3& rot)
	{

	}
	const void  Transform::SetScale(const glm::f32vec3& scale)
	{

	}

	const void  Transform::SetPosition(const glm::f32& x, const glm::f32& y, const glm::f32& z)
	{
	}
	const void  Transform::SetRotation(const glm::f32& x, const glm::f32& y, const glm::f32& z)
	{
	}
	const void  Transform::SetScale(const glm::f32& x, const glm::f32& y, const glm::f32& z)
	{

	}

	const void  Transform::SetPositionX(const glm::f32& x)
	{
	}
	const void  Transform::SetPositionY(const glm::f32& y)
	{
	}
	const void  Transform::SetPositionZ(const glm::f32& z)
	{
	}
}