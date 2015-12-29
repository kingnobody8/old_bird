#include "renderer.h"
#include "math/math.h"
#include "color.h"
#include <string>
#include "func.h"
#include "macro.h"
#include "util_time.h"
#include "render_layer.h"
#include "camera.h"

#define GLEW_STATIC

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

bool gRenderQuad = true;

namespace engine
{
	namespace render
	{
		static SDL_Renderer* s_sdlRen = null;
		static SDL_Window* s_sdlWin = null;
		const glm::u32vec2 default_dims = glm::u32vec2(1280, 720);
		const std::string app_name = "The Lark Ascending";
		const int GRID_CELL_SIZE = 64;

		//OpenGL context
		SDL_GLContext gContext;

		//Graphics program
		GLuint gProgramID = 0;
		GLint gVertexPos2DLocation = -1;
		GLuint gVBO = 0;
		GLuint gIBO = 0;

		void printProgramLog(GLuint program)
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

		void printShaderLog(GLuint shader)
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

		bool initGL()
		{

			//Success flag
			bool success = true;

			//Generate program
			gProgramID = glCreateProgram();

			//Create vertex shader
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

			//Get vertex source
			const GLchar* vertexShaderSource[] =
			{
				"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
			};

			//Set vertex source
			glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

			//Compile vertex source
			glCompileShader(vertexShader);

			//Check vertex shader for errors
			GLint vShaderCompiled = GL_FALSE;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
			if (vShaderCompiled != GL_TRUE)
			{
				printf("Unable to compile vertex shader %d!\n", vertexShader);
				printShaderLog(vertexShader);
				success = false;
			}
			else
			{
				//Attach vertex shader to program
				glAttachShader(gProgramID, vertexShader);


				//Create fragment shader
				GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

				//Get fragment source
				const GLchar* fragmentShaderSource[] =
				{
					"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
				};

				//Set fragment source
				glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

				//Compile fragment source
				glCompileShader(fragmentShader);

				//Check fragment shader for errors
				GLint fShaderCompiled = GL_FALSE;
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
				if (fShaderCompiled != GL_TRUE)
				{
					printf("Unable to compile fragment shader %d!\n", fragmentShader);
					printShaderLog(fragmentShader);
					success = false;
				}
				else
				{
					//Attach fragment shader to program
					glAttachShader(gProgramID, fragmentShader);


					//Link program
					glLinkProgram(gProgramID);

					//Check for errors
					GLint programSuccess = GL_TRUE;
					glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
					if (programSuccess != GL_TRUE)
					{
						printf("Error linking program %d!\n", gProgramID);
						printProgramLog(gProgramID);
						success = false;
					}
					else
					{
						//Get vertex attribute location
						gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
						if (gVertexPos2DLocation == -1)
						{
							printf("LVertexPos2D is not a valid glsl program variable!\n");
							success = false;
						}
						else
						{
							//Initialize clear color
							glClearColor(0.f, 0.f, 0.f, 1.f);

							//VBO data
							GLfloat vertexData[] =
							{
								-0.5f, -0.5f,
								0.5f, -0.5f,
								0.5f, 0.5f,
								-0.5f, 0.5f
							};

							//IBO data
							GLuint indexData[] = { 0, 1, 2, 3 };

							//Create VBO
							glGenBuffers(1, &gVBO);
							glBindBuffer(GL_ARRAY_BUFFER, gVBO);
							glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

							//Create IBO
							glGenBuffers(1, &gIBO);
							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
							glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
						}
					}
				}
			}

			return success;
		}



		void RenderGrid(vec2 half_size) __todo() //make some flags that engine can set in the renderer to turn these on and off also maybe choose between foreground and background
		{
			/*render::CRenderNodeRect node;
			node.SetColor(util::Color::CYAN);
			node.SetFill(false);
			util::shape::AABB tmp;*/

			//Grid
			/*for (int x = -half_size.x; x < half_size.x; x += GRID_CELL_SIZE)
			{
				for (int y = -half_size.y; y < half_size.y; y += GRID_CELL_SIZE)
				{
					tmp.m_min = util::math::vec2((float)x, (float)y);
					tmp.m_max = util::math::vec2((float)x + GRID_CELL_SIZE, (float)y + GRID_CELL_SIZE);
					node.SetAABB(tmp);
					node(s_sdlRen, util::math::Matrix2D());
				}
			}*/
		}
		void RenderCrossSection(glm::u32vec2 logical_size)
		{
			SDL_SetRenderDrawColor(s_sdlRen, 255, 255, 255, 255);
			//X-axis
			SDL_RenderDrawLine(s_sdlRen,
				0, (int)(logical_size.y * 0.5f),
				logical_size.x, (int)(logical_size.y * 0.5f));
			//Y-axis
			SDL_RenderDrawLine(s_sdlRen,
				(int)(logical_size.x * 0.5f), 0,
				(int)(logical_size.x * 0.5f), logical_size.y);
			//topleft to botright
			SDL_RenderDrawLine(s_sdlRen,
				0, 0,
				logical_size.x, logical_size.y);
			//botleft to topright
			SDL_RenderDrawLine(s_sdlRen,
				0, logical_size.y,
				logical_size.x, 0);
		}

		void SetupSdl()
		{
			//Create Window
#ifdef MOBILE
			SDL_DisplayMode mode;
			SDL_GetCurrentDisplayMode(0, &mode);
			//this->m_tPhysicalScreenSize = Type2<slong>(mode.w, mode.h);
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
			s_sdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, win_flag);
#else
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
			s_sdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, default_dims.x, default_dims.y, win_flag);
#endif
			//Error check
			util::CheckSdlError();
			assert(s_sdlWin);

			__todo() // figure out how to use open gl with SDL
				//Create an OpenGL context
				//SDL_GLContext pContext = NULL;// SDL_GL_CreateContext(pWindow);
				//if (!pContext)
				//{
				//	SDL_Log("Error - SDL_GL_CreateContext - %s.\n", SDL_GetError());
				//	return 3;
				//}
				SDL_Log("SDL Window Initialized");

			//Use OpenGL 3.1 core
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


			//Create context
			gContext = SDL_GL_CreateContext(s_sdlWin);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				//Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
				}
			}
		}
		void Destroy()
		{
			CRenderLayer::DestroyLayers();
			CCamera::DestroyCameras();
		}
		SDL_Renderer* GetSdlRenderer() { return s_sdlRen; }
		SDL_Window* GetSdlWindow() { return s_sdlWin; }
		void DoRender()
		{
			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			//Render quad
			if (gRenderQuad)
			{
				//Bind program
				glUseProgram(gProgramID);

				//Enable vertex position
				glEnableVertexAttribArray(gVertexPos2DLocation);

				//Set vertex data
				glBindBuffer(GL_ARRAY_BUFFER, gVBO);
				glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

				//Set index data and render
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
				glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

				//Disable vertex position
				glDisableVertexAttribArray(gVertexPos2DLocation);

				//Unbind program
				glUseProgram(NULL);

				//Update screen
				SDL_GL_SwapWindow(s_sdlWin);
			}

			return;

			//Primer
			SDL_RenderSetClipRect(s_sdlRen, null);//clear out any scissor rects
			util::Color background = util::Color::other::AUBURN;
			background.r = background.g = background.b = 76;
			background.a = 255;
			SDL_SetRenderDrawColor(s_sdlRen, background.r, background.g, background.b, background.a);
			SDL_SetRenderTarget(s_sdlRen, NULL);
			SDL_RenderClear(s_sdlRen);

			glm::i32vec2 logical_size;
			SDL_GetRendererOutputSize(s_sdlRen, &logical_size.x, &logical_size.y);
			glm::i32vec2 half_size = logical_size / 2;
			glm::i32vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			//RenderGrid(half_size);
			RenderCrossSection(logical_size);

			SDL_SetRenderDrawColor(s_sdlRen, 32, 32, 32, 255);

			//	render::CRenderNodeLine line;
			//	line.SetColor(util::Color::WHITE.SDL());
			//	util::shape::Segment seg;
			//	seg.start = node.GetAABB().m_max;
			//	seg.end = node.GetAABB().m_min;
			//	line.SetLine(seg);


			////	const util::Time t = util::Time::GetTimeSinceEpoch();

			//	util::math::Matrix2D cam =util::math::Matrix2D(vec2(0,0), vec2(1,1), 0);
			//	const util::math::Matrix2D inv_cam = util::math::Matrix2D::Matrix_Inverse(cam);

			//	//node(s_sdlRen, inv_cam);
			//	line(s_sdlRen, inv_cam);
			//	node.SetAABB(line.CalcAABB());
			//	node(s_sdlRen, inv_cam);

			CRenderLayer::RenderAllLayers(s_sdlRen);

			//Present
			SDL_RenderPresent(s_sdlRen);
		}
	}
}