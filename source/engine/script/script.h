#pragma once

namespace engine
{
	namespace script
	{
		enum EPriorities
		{
			FIRST_PART = 1,
			RECT_PART,
			IMG_PART,
			PRIORITIES_COUNT
		};

		void RegisterScripts();
	}
}