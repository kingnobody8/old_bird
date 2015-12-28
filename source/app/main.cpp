/*
File:		main.cpp
Purpose:	Crossplatform Main Interface
*/

#include "sdl/include/sdl.h"

#include "../utility/bit_flag.h"
#include "../utility/color.h"
#include "../utility/enums.h"
#include "../utility/func.h"
#include "../utility/json.h"
#include "../utility/macro.h"
#include "../utility/math/consts.h"
#include "../utility/math/interp.h"
#include "../utility/math/math.h"
#include "../utility/common.h"
#include "../utility/platform.h"
#include "../utility/timer.h"
#include "../utility/types.h"
#include "../utility/util_time.h"

#include "../engine/engine.h"
#include "../engine/component/object.h"
#include "../engine/component/part.h"
#include "../engine/component/component.h"

#include "../engine/asset/loader.h"
#include "../engine/asset/resource_path.h"


//#include "platform/inc_sdl.h"
//#include "util_time.h"
//#include "macro.h"
//#include "bit_flag.h"
//#include "func.h"
//#include "timer.h"
//#include <vector>
using namespace util;

//#include "engine/asdf.h"
//#include "engine/engine.h"
//#include "app/state/intro_state.h"

//#include "test.h"
//#include <Box2D/Box2D.h>

#ifdef WIN
//#include <vld.h>
#endif

int main(int argc, char** argv)
{
	engine::Engine* engi = engine::Get();

	engi->Init(null);
	
#ifdef WIN
	while (!engi->GetQuit())
	{
		engi->RunFrame(null);
	}
	engi->Exit();
#endif
	return 0;
}
