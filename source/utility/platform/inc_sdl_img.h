#pragma once
#include "platform/platform.h"

// 2015y:09m:04d:22h:03m:46s:32
// 15:09:04:22:03:46:32
// YYYY:MM:DD:HH:MM:SS:MS
// 2015:09:04:22:03:46:32

#ifdef WIN
#include <SDL_image.h>
#elif MAC
#include <SDL2/SDL_image.h>
#elif IOS_SIM
#include "SDL_image.h"
#elif IOS_DEV
#include "SDL_image.h"
#endif