#include "script.h"
#include "script/renderable/render_part.h"
#include "script/renderable/rect_part.h"
#include "script/renderable/img_part.h"

namespace engine
{
	namespace script
	{
		//if a script hides behind a namespace we have to set using here so that the register macro can take just the straight class name
		__todo() //maybe eventually we want to organize things like bvb did with namespaces as part of the part name ie: engine.render.crectpart or engine.ui.button
		using namespace renderable;

#define REGISTER_PART(CLASS) engine::component::IPart::RegisterPart(#CLASS, CLASS::Type, util::CNewType<CLASS>())

		void RegisterScripts()
		{
			REGISTER_PART(CRectPart);
			REGISTER_PART(CImgPart);
		}
	}
}