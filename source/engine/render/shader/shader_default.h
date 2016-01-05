#pragma once
#include "shader_program.h"

namespace engine
{
	namespace render
	{
		class DefaultShader : public IShaderProgram
		{
		protected:
			GLint m_vertexPos2DLocation;

		public:
			DefaultShader();
			virtual const bool LoadProgram();

			void EnableVertexPos2D();
			void SetVertexPos2D(GLsizei stride, const GLvoid* data);
			void DisableVertexPos2D();

		};
	}
}