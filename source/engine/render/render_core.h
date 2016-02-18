#pragma once
#include "platform.h"

#include <SDL.h>

#if WIN
#include <Windows.h>
#include <GL/gl.h>
#include "bridge/win/GL/glext.h"
#include "bridge/win/bridge-gl-win.h"
#elif MAC
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include "bridge/mac/bridge-gl-mac.h"
#elif IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

