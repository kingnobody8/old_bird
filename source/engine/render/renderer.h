#pragma once
#include "sdl/include/SDL.h"
#include "types.h"

namespace engine
{
	namespace render
	{
		void Setup();
		void Destroy();
		SDL_GLContext GetGLContext();
		SDL_Window* GetSdlWindow();
		void DoRender();
	}
}