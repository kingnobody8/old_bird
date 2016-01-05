#include "renderer.h"
#include "math/math.h"
#include "color.h"
#include <string>
#include "func.h"
#include "macro.h"
#include "util_time.h"
#include "render_layer.h"
#include "camera.h"
#include "shader.h"

#include "opengl.h"

#include "node/polygon_node.h"
#include "shader/shader_color_polygon.h"
#include "shader/shader_default.h"

bool gRenderQuad = true;

namespace engine
{
	namespace render
	{
		__todo() //add handle window resize

		static SDL_Window* s_sdlWin = null;
		static SDL_GLContext s_glContext = null;
		const glm::u32vec2 default_dims = glm::u32vec2(1280, 720);
		const std::string app_name = "The Lark Ascending"; __todo() //move this to app project same for screen size
			const int GRID_CELL_SIZE = 64;

		//----------------------------------------
		DefaultShader shader;
		PolygonNode node;
		//-----------------------------------------

		void RenderTestFunc()
		{
			node.SetShader(&shader);
			node(matrix());
		}

		bool initGL()
		{
			shader.LoadProgram();

			//Initialize clear color
			glClearColor(0.13f, 0.09f, 0.1f, 1.0f);

			std::vector<VertexColor> verts;
			verts.push_back(VertexColor(vec2(-1.0f, -1.0f)));
			verts.push_back(VertexColor(vec2(0.5f, -0.5f)));
			verts.push_back(VertexColor(vec2(1.0f, 1.0f)));
			verts.push_back(VertexColor(vec2(-0.5f, 0.5f)));

			std::vector<int> indicies;
			indicies.push_back(0);
			indicies.push_back(1);
			indicies.push_back(2);
			indicies.push_back(3);

			node.InitVBO(verts, indicies);

			return true;
		}

		void RenderGrid() __todo() //make some flags that engine can set in the renderer to turn these on and off also maybe choose between foreground and background
		{
			vec2 half_size;
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
		void RenderCrossSection()
		{
			glm::u32vec2 logical_size;
			//SDL_SetRenderDrawColor(s_sdlRen, 255, 255, 255, 255);
			////X-axis
			//SDL_RenderDrawLine(s_sdlRen,
			//	0, (int)(logical_size.y * 0.5f),
			//	logical_size.x, (int)(logical_size.y * 0.5f));
			////Y-axis
			//SDL_RenderDrawLine(s_sdlRen,
			//	(int)(logical_size.x * 0.5f), 0,
			//	(int)(logical_size.x * 0.5f), logical_size.y);
			////topleft to botright
			//SDL_RenderDrawLine(s_sdlRen,
			//	0, 0,
			//	logical_size.x, logical_size.y);
			////botleft to topright
			//SDL_RenderDrawLine(s_sdlRen,
			//	0, logical_size.y,
			//	logical_size.x, 0);
		}

		void Setup()
		{
			//Create Window
#ifdef MOBILE
			SDL_DisplayMode mode;
			SDL_GetCurrentDisplayMode(0, &mode);
			//this->m_tPhysicalScreenSize = Type2<slong>(mode.w, mode.h);
			Uint32 win_flag = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
			s_sdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, win_flag);
#else
			Uint32 win_flag = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
			s_sdlWin = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, default_dims.x, default_dims.y, win_flag);
#endif
			//Error check
			util::CheckSdlError();
			assert(s_sdlWin);
			SDL_Log("SDL Window Initialized");

			//Use OpenGL 3.1 core
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			//Create context
			s_glContext = SDL_GL_CreateContext(s_sdlWin);
			util::CheckSdlError();
			assert(s_glContext);
			SDL_Log("Open GL Context Initialized");

			//Initialize GLEW
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (glewError != GLEW_OK)
			{
				SDL_Log((char*)glewGetErrorString(glewError));
				assert(false);
			}
			SDL_Log("GLEW Initialized");

			//Use Vsync
			if (SDL_GL_SetSwapInterval(1) < 0)
			{
				util::CheckSdlError();
				assert(false);
			}
			SDL_Log("VSYNC Initialized");

			SDL_Log("Renderer Initialized");
			//--------------------------------------

			initGL();

			//std::vector<VertexColor> verts;
			//std::vector<int> indicies;

			//verts.resize(4);
			//verts[0].position = vec2(-0.5f, -0.5f);
			//verts[1].position = vec2(0.5f, -0.5f);
			//verts[2].position = vec2(0.5f, 0.5f);
			//verts[3].position = vec2(-0.5f, 0.5f);

			//for (int i = 0; i < 4; ++i)
			//{
			////	verts[i].color = util::Color::WHITE;
			//}

			//indicies.resize(4);
			//indicies[0] = 0;
			//indicies[1] = 1;
			//indicies[2] = 2;
			//indicies[3] = 3;

			//node.InitVBO(verts, indicies);

			//render::RegisterShaders();

			//shader = IShaderProgram::CreateShader(ShaderColorPolygon::Type);
			//node.SetShader(shader);
		}
		void Destroy()
		{
			CRenderLayer::DestroyLayers();
			CCamera::DestroyCameras();
			SDL_DestroyWindow(s_sdlWin);
			s_sdlWin = null;
		}
		SDL_GLContext GetGLContext() { return s_glContext; }
		SDL_Window* GetSdlWindow() { return s_sdlWin; }
		void DoRender()
		{
			if (s_sdlWin == null)
				return;

			//Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT);

			//RenderGrid();
			RenderCrossSection();
			RenderTestFunc();

			CRenderLayer::RenderAllLayers();

			//Update screen
			SDL_GL_SwapWindow(s_sdlWin);
		}
	}
}