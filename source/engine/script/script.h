#pragma once

namespace engine
{
	namespace script
	{
#define REGISTER_PART(CLASS) engine::component::IPart::RegisterPart(#CLASS, CLASS::Type, util::CNewType<CLASS>())

		enum EPriorities
		{
			FIRST_PART = 1,
			AABB_PART,
			IMG_PART,
			PRIORITIES_COUNT
		};

		void RegisterScripts();
	}
}