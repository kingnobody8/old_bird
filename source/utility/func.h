/*
Author:		Daniel Habig
Date:		7-16-14
File:		Funcs.h
Purpose:	These are commonly used utility functions and macros
*/

#pragma once
#include <cmath>
#include <stdlib.h>
#include "types.h"
#include <assert.h>
#include "sdl.h"
#include <string>

#ifndef Max
#define Max(a,b)					(((a) > (b)) ? (a) : (b))					//Returns the maximum between 2 numbers
#endif
#ifndef Min
#define Min(a,b)					(((a) < (b)) ? (a) : (b))					//Returns the minimum between 2 numbers
#endif
#ifndef Clamp
#define Clamp(val, floor, ceiling)	(Max(floor, Min(val, ceiling)))				//Clamps a value to be between the floor and ceiling
#endif
#ifndef Damp
#define Damp(val, delta)			((val > delta) (val -= delta) : (val = 0))	//Pushes a value towards 0 by the delta specified, stops when it reaches 0
#endif
#ifndef Lerp
#define Lerp(alpha, omega, lerp)	(alpha + (omega - alpha) * lerp)			//performs a linear interpolation
#endif
#ifndef Odd
#define Odd(num)					(num % 2 != 0)
#endif
#ifndef Even
#define Even(num)					(!Odd(num))
#endif
#ifndef SafeDelete
#define SafeDelete(param)					\
						if(param != null)	\
						{ delete param; }	\
						param = null;
#endif

namespace util
{
	static slong RandomSlong(slong min, slong max)
	{
		return rand() % ulong(max - min + 1) + min;
	}
	static float RandomFloat(float min, float max)
	{
		return (max - min) * (float(rand()) / float(RAND_MAX)) + min;
	}

	static void CheckSdlError(const char* ignore_error = null)
	{
		const char* err = SDL_GetError();
		if (*err != null)
			SDL_Log("SDL Error: %s", err);
		assert(*err == null || std::strcmp(err, ignore_error) == 0);
		SDL_ClearError();
	}

	bool BigEndianCheck(void);
	void TypeCheck(void);

	//Functor CNewType - returns a new instance of the defined type
	template<typename type>
	class CNewType
	{
	public:
		type* operator () (void)
		{
			return new type();
		}
	};
}
