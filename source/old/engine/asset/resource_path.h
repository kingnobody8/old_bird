#pragma once
#include "engine/platform/platform.h"
#include <string>


#ifdef WIN
		static std::string getResourcePath(void) { return "../../../"; }
#elif APPLE
		std::string getResourcePath(void);
#endif

