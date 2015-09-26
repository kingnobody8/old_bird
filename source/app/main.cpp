/*
File:		main.cpp
Purpose:	Crossplatform Main Interface
*/

#include "../utility/bit_flag.h"
#include "../utility/color.h"
#include "../utility/enums.h"
#include "../utility/func.h"
#include "../utility/json.h"
#include "../utility/macro.h"
#include "../utility/math/consts.h"
#include "../utility/math/interp.h"
#include "../utility/math/matrix_2d.h"
#include "../utility/math/matrix_stack.h"
#include "../utility/math/type2.h"
#include "../utility/math/type3.h"
#include "../utility/math/type4.h"
#include "../utility/platform/inc_common.h"
#include "../utility/platform/inc_func.h"
#include "../utility/platform/inc_sdl.h"
#include "../utility/platform/inc_sdl_img.h"
#include "../utility/platform/platform.h"
#include "../utility/timer.h"
#include "../utility/types.h"
#include "../utility/util_time.h"

#include "../engine/engine.h"
#include "../engine/component/object.h"
#include "../engine/component/part.h"
#include "../engine/component/component.h"



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
	while (!engi->GetQuit())
	{
		engi->Update();
	}
	engi->Exit();
	return 0;
}
