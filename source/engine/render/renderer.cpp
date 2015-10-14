#include "renderer.h"
#include "math/type2.h"
#include "color.h"
#include <string>
#include "func.h"
#include "macro.h"
#include "render_node.h"
#include "util_time.h"
#include "render_layer.h"

namespace engine
{
	namespace render
	{
		static SDL_Renderer* s_sdlRen = null;
		static SDL_Window* s_sdlWin = null;
		const util::math::Type2<int> default_dims = util::math::Type2<int>(1280, 720);
		const std::string app_name = "The Lark Ascending";

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

			render::CRenderNodeRect node;
			node.SetColor(util::Color::WHITE.SDL());
			node.SetFill(false);
			util::shape::AABB tmp;
			tmp.m_min = util::math::vec2(logical_size.w * -0.25f, logical_size.h * -0.25f);
			tmp.m_max = util::math::vec2(logical_size.w * 0.25f, logical_size.h * 0.25f);
			node.SetAABB(tmp);
			node(s_sdlRen, util::math::Matrix2D());

			SDL_RenderDrawLine(s_sdlRen,
				0, 0,
				logical_size.x, logical_size.y);

			SDL_RenderDrawLine(s_sdlRen,
				0, logical_size.y,
				logical_size.x, 0);

			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);
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