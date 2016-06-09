/*
File:		main.cpp
Purpose:	Crossplatform Main Interface
*/

#include "engine/engine.h"
#include "state/setup_state.h"

#include "sdl.h"

#ifdef WIN
#include <vld.h>
#endif

int main(int argc, char** argv)
{
	engine::Engine* engi = engine::Engine::Get();

	engi->Init(new app::state::SetupState());
	
#ifndef IS_IOS
	while (!engi->GetQuit())
	{
		engi->RunFrame(null);
	}
	engi->Exit();
#endif
	return 0;
}
