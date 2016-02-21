#pragma once

#ifdef _WIN64 //define something for Windows (64-bit)
#define WIN 1
#define PC 1
#define x64 1
#elif _WIN32 //define something for Windows (32-bit)
#define WIN 1
#define PC 1
#define x32 1
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR // define something for simulator
#define IOS_SIM 1
#define IOS 1
#define MOBILE 1
#define APPLE 1
#define x64 1
#elif TARGET_OS_IPHONE // define something for iphone
#define IOS_DEV 1
#define IOS 1
#define MOBILE 1
#define APPLE 1
#define x64 1
#else
#define TARGET_OS_OSX 1// define something for OSX
#define MAC 1
#define PC 1
#define APPLE 1
#define x64 1
#endif
#elif __linux	// linux
#elif __unix	// all unices not caught above // Unix
#elif __posix	// POSIX
#endif