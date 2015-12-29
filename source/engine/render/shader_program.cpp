#include "shader_program.h"
#include <fstream>
#include "macro.h"

namespace engine
{
	namespace render
	{

		IShaderProgram::IShaderProgram()
			: m_programID(0)
		{
		}

		IShaderProgram::~IShaderProgram()
		{
			FreeProgram();
		}

		void IShaderProgram::FreeProgram()
		{
			glDeleteProgram(m_programID);
			__todo() //check that this also resets the m_programID to 0
		}

		const bool IShaderProgram::Bind()
		{
			//Use shader
			glUseProgram(m_programID);

			//Check for error
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				printf("Error binding shader! %s\n", gluErrorString(error));
				PrintProgramLog(m_programID);
				return false;
			}

			return true;
		}

		void IShaderProgram::Unbind()
		{
			//Use default program
			glUseProgram(NULL);
		}

		void IShaderProgram::PrintProgramLog(GLuint program)
		{
			//Make sure name is shader
			if (glIsProgram(program))
			{
				//Program log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;

				//Get info string length
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				//Allocate string
				char* infoLog = new char[maxLength];

				//Get info log
				glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
				if (infoLogLength > 0)
				{
					//Print Log
					printf("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else
			{
				printf("Name %d is not a program\n", program);
			}
		}

		void IShaderProgram::PrintShaderLog(GLuint shader)
		{
			//Make sure name is shader
			if (glIsShader(shader))
			{
				//Shader log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;

				//Get info string length
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				//Allocate string
				char* infoLog = new char[maxLength];

				//Get info log
				glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
				if (infoLogLength > 0)
				{
					//Print Log
					printf("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else
			{
				printf("Name %d is not a shader\n", shader);
			}
		}

		const GLuint IShaderProgram::LoadShaderFromFile(const std::string& path, const GLenum& shaderType)
		{
			//Open file
			GLuint shaderID = 0;
			std::string shaderString;
			std::ifstream sourceFile(path.c_str());

			//Source file loaded
			if (sourceFile)
			{
				//Get shader source
				shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

				//Load the shader source
				shaderID = LoadShaderFromString(shaderString, shaderType);
			}
			else
			{
				printf("Unable to open file %s\n", path.c_str());
			}

			return shaderID;
		}

		const GLuint IShaderProgram::LoadShaderFromString(const std::string& shaderString, const GLenum& shaderType)
		{
			GLuint shaderID = 0;

			//Create shader ID
			shaderID = glCreateShader(shaderType);

			//Set shader source
			const GLchar* shaderSource = shaderString.c_str();
			glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

			//Compile shader source
			glCompileShader(shaderID);

			//Check shader for errors
			GLint shaderCompiled = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
			if (shaderCompiled != GL_TRUE)
			{
				printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
				PrintShaderLog(shaderID);
				glDeleteShader(shaderID);
				shaderID = 0;
			}

			return shaderID;
		}
	}
}