#pragma once
#include "util_math.h"
#include "angle.h"

namespace util
{
	class Transform
	{
	public:
		Transform();
		Transform(const vec3& pos, const vec3& rot, const vec3& scale);

		inline const vec3 GetPosition() const { return m_pos; }
		inline const vec3 GetRotation() const { return m_rotation; }
		inline const vec3 GetScale() const { return m_scale; }

		const void SetPosition(const vec3& pos);
		const void SetRotation(const vec3& rot);
		const void SetScale(const vec3& scale);

		const void SetPosition(const glm::f32& x, const glm::f32& y, const glm::f32& z);
		const void SetRotation(const glm::f32& x, const glm::f32& y, const glm::f32& z);
		const void SetScale(const glm::f32& x, const glm::f32& y, const glm::f32& z);

		const void SetPositionX(const glm::f32& x);
		const void SetPositionY(const glm::f32& y);
		const void SetPositionZ(const glm::f32& z);

		inline const matrix GetMatrix() const { return m_matrix; }

	protected:
		matrix m_matrix;

		vec3 m_pos;
		vec3 m_rotation;
		vec3 m_scale;

		bool m_dirty;

	};

	class Transform2D : private Transform
	{
		inline const vec2 GetPosition() const { return vec2(m_pos.x, m_pos.y); }
		inline const Angle GetRotation() const { return Angle(m_rotation.z); }
		inline const vec2 GetScale() const { return vec2(m_scale.x, m_scale.y); }
	};
}
