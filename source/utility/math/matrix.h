#pragma once
#include "glm/glm.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace util
{
	namespace math
	{
		class Matrix : public glm::f32mat4x4
		{
		public:
			Matrix() {}
			Matrix(const glm::f32mat4x4& that) { *this = that; }
			Matrix& operator= (const glm::f32mat4x4& that);

			const glm::f32vec3 GetPosition() const;
			const glm::f32vec3 GetRotation() const;
			const glm::f32vec3 GetScale() const;

			const void SetPosition(const glm::f32vec3& pos);
			const void SetRotation(const glm::f32vec3& rot);
			const void SetScale(const glm::f32vec3& scale);

			const void SetPosition(const glm::f32& x, const glm::f32& y, const glm::f32& z);
			const void SetRotation(const glm::f32& x, const glm::f32& y, const glm::f32& z);
			const void SetScale(const glm::f32& x, const glm::f32& y, const glm::f32& z);

			const void SetPositionX(const glm::f32& x);
			const void SetPositionY(const glm::f32& y);
			const void SetPositionZ(const glm::f32& z);
		};
	}
}
