#include "shader_color_polygon.h"

namespace engine
{
	namespace render
	{
		DEFINE_SHADER_TYPE_INFO(ShaderColorPolygon);

		ShaderColorPolygon::ShaderColorPolygon()
			: m_vertexPositionLocation(0)
			, m_vertexColorLocation(0)
			, m_projectionMatrixLocation(0)
			, m_modelViewMatrixLocation(0)
		{
		}

		VIRTUAL ShaderColorPolygon::~ShaderColorPolygon()
		{
		}

		VIRTUAL const bool ShaderColorPolygon::LoadProgram()
		{
			//Generate program
			m_programID = glCreateProgram();

			//Load vertex shader
			GLuint vertexShader = LoadShaderFromFile("shader/shader_color_polygon.glvs", GL_VERTEX_SHADER);

			//Check for errors
			if (vertexShader == 0)
			{
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}

			//Attach vertex shader to program
			glAttachShader(m_programID, vertexShader);


			//Create fragment shader
			GLuint fragmentShader = LoadShaderFromFile("shader/shader_color_polygon.glfs", GL_FRAGMENT_SHADER);

			//Check for errors
			if (fragmentShader == 0)
			{
				glDeleteShader(vertexShader);
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}

			//Attach fragment shader to program
			glAttachShader(m_programID, fragmentShader);

			//Link program
			glLinkProgram(m_programID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(m_programID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", m_programID);
				PrintProgramLog(m_programID);
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
				glDeleteProgram(m_programID);
				m_programID = 0;
				return false;
			}

			//Clean up excess shader references
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			//Get variable locations
			m_vertexPositionLocation = glGetAttribLocation(m_programID, "n_vertexPosition");
			assert(m_vertexPositionLocation != -1);

			m_vertexColorLocation = glGetAttribLocation(m_programID, "n_vertexColor");
			assert(m_vertexColorLocation != -1);

			m_projectionMatrixLocation = glGetUniformLocation(m_programID, "u_projectionMatrix");
			assert(m_projectionMatrixLocation != -1);

			m_modelViewMatrixLocation = glGetUniformLocation(m_programID, "u_modelViewMatrix");
			assert(m_modelViewMatrixLocation != -1);

			return true;
		}
	}
}

