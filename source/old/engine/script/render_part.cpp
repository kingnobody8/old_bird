#include "render_part.h"
#include "engine/component/object.h"
#include <Box2D/Collision/Shapes/b2Shape.h>

namespace Engine
{
	namespace Script
	{
		IRenderPart::IRenderPart(void)
		{
			//this->m_szType = "IRenderPart";
			this->m_priority = Part::EPriorities::IMG;
		}
		IRenderPart::~IRenderPart(void)
		{
			/*LEAVE EMPTY - ABC*/
		}

		/*virtual*/ void IRenderPart::LoadJson(const Json::JSON& json)
		{
			this->m_szLayer = json["layer"].GetString();
		}
	}
}
