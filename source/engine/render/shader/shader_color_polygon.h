#pragma once
#include "shader_program.h"

namespace engine
{
	namespace render
	{
		__todo() //move all shaders into application project, unless we use them as an enum typed
		class ShaderColorPolygon : public IShaderProgram
		{
		public:
			DECLARE_SHADER_TYPE_INFO(ShaderColorPolygon);

			ShaderColorPolygon();
			virtual ~ShaderColorPolygon();
			virtual const bool LoadProgram();

			void SetVertexPositionPointer(GLsizei stride, const GLvoid* data);
			void SetVertexColorPointer(GLsizei stride, const GLvoid* data);

			void DisableVertexPositionPointer();
			void DisableVertexColorPointer();

			void SetProjectionMatrix(const matrix& projectionMat);
			void SetModelViewMatrix(const matrix& modelViewMat);

		protected:
			//Attribute locations
			GLint m_vertexPositionLocation;
			GLint m_vertexColorLocation;

			//Projection matrix
			matrix m_projectionMatrix;
			GLint m_projectionMatrixLocation;

			//Modelview matrix
			matrix m_modelViewMatrix;
			GLint m_modelViewMatrixLocation;

		};
	}
}