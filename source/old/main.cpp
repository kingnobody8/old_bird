/*
Author:		Daniel Habig
Date:		3-11-15
File:		main.cpp
Purpose:	Crossplatform Main Interface
*/
#include "engine/platform/inc_sdl.h"
#include "engine/asdf.h"
#include "engine/engine.h"
#include "app/state/intro_state.h"

#include "test.h"
//#include <Box2D/Box2D.h>

#ifdef WIN
#include <vld.h>
#endif

int main(int argc, char** argv)
{
	int integer = generate(); // = 17
	std::string str = generate(); // = "foo" //TODO check and make sure this works in clang
	float f = generate(); // = 3.14159f

	Engine::CEngine* engine = Engine::CEngine::Get();
	engine->Init(new App::StateMachine::CIntroState());
	engine->Run();
#if PC
	engine->Exit();
#endif
	return 0;
}
