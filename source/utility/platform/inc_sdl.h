#pragma once
#include "platform/platform.h"

#ifdef WIN
#include <SDL.h>
#elif MAC
#include <SDL2/SDL.h>
#elif IOS_SIM
#include "SDL.h"
#elif IOS_DEV
#include "SDL.h"
#endif