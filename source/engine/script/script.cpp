#include "script.h"
#include "script/renderable/render_part.h"
#include "script/renderable/aabb_part.h"
#include "script/renderable/points_part.h"
#include "script/renderable/img_part.h"

namespace engine
{
	namespace script
	{
		//if a script hides behind a namespace we have to set using here so that the register macro can take just the straight class name
		__todo() //maybe eventually we want to organize things like bvb did with namespaces as part of the part name ie: engine.render.crectpart or engine.ui.button
		using namespace renderable;

		void RegisterScripts()
		{
			REGISTER_PART(CAabbPart);
			REGISTER_PART(CPointsPart);
			REGISTER_PART(CImgPart);
		}
	}
}