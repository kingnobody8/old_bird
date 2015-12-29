#pragma once
#include "opengl.h"
#include <string>

namespace engine
{
	namespace render
	{
		class IShaderProgram
		{
		protected:
			GLuint m_programID;


		public:
			IShaderProgram();
			virtual ~IShaderProgram();

			virtual const bool LoadProgram() = 0;
			virtual void FreeProgram();
			const bool Bind();
			void Unbind();
			const GLuint GetProgramID() { return m_programID; }

		protected:
			void PrintProgramLog(GLuint program);
			void PrintShaderLog(GLuint shader);
			const GLuint LoadShaderFromFile(const std::string& path, const GLenum& shaderType);
			const GLuint LoadShaderFromString(const std::string& shaderString, const GLenum& shaderType);
		};
	}
}