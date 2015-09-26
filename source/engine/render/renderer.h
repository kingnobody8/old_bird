#pragma once
#include "platform/inc_sdl.h"
#include "types.h"

namespace engine
{
	namespace render
	{
		void SetupSdl();
		SDL_Renderer* GetSdlRenderer();
		SDL_Window* GetSdlWindow();
		void DoRender();
	}
}