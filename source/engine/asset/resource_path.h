#pragma once
#include "platform/platform.h"
#include <string>


#ifdef WIN
		static const std::string getResourcePath(void) { return "../../../"; }
#elif APPLE
		std::string getResourcePath(void);
#endif

