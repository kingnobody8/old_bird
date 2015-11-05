#include "renderer.h"
#include "math/type2.h"
#include "color.h"
#include <string>
#include "func.h"
#include "macro.h"
#include "render_node.h"
#include "util_time.h"
#include "render_layer.h"
#include "camera.h"

namespace engine
{
	namespace render
	{
		static SDL_Renderer* s_sdlRen = null;
		static SDL_Window* s_sdlWin = null;
		const util::math::Type2<int> default_dims = util::math::Type2<int>(1280, 720);
		const std::string app_name = "The Lark Ascending";
		const int GRID_CELL_SIZE = 64;


		void RenderGrid(util::math::Type2<int> half_size) __todo() //make some flags that engine can set in the renderer to turn these on and off also maybe choose between foreground and background
		{
			render::CRenderNodeRect node;
			node.SetColor(util::Color::CYAN.SDL());
			node.SetFill(false);
			util::shape::AABB tmp;

			//Grid
			for (int x = -half_size.x; x < half_size.x; x += GRID_CELL_SIZE)
			{
				for (int y = -half_size.y; y < half_size.y; y += GRID_CELL_SIZE)
				{
					tmp.m_min = util::math::vec2(x, y);
					tmp.m_max = util::math::vec2(x + GRID_CELL_SIZE, y + GRID_CELL_SIZE);
					node.SetAABB(tmp);
					node(s_sdlRen, util::math::Matrix2D());
				}
			}
		}
		void RenderCrossSection(util::math::Type2<int> logical_size)
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

			//Create SDL Renderer
			s_sdlRen = SDL_CreateRenderer(s_sdlWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			//Error check
			util::CheckSdlError("Setting the swap interval is not supported");
			assert(s_sdlRen);
			SDL_Log("SDL Renderer Initialized");

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
			//Primer
			const SDL_Color background = util::Color::other::AUBURN.SDL();
			SDL_SetRenderDrawColor(s_sdlRen, background.r, background.g, background.b, background.a);
			SDL_SetRenderTarget(s_sdlRen, NULL);
			SDL_RenderClear(s_sdlRen);

			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(s_sdlRen, &logical_size.w, &logical_size.h);
			util::math::Type2<int> half_size = logical_size / 2;
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

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