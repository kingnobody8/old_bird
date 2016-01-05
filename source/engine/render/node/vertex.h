#pragma once
#include "math/math.h"

namespace engine
{
	namespace render
	{
		struct VertexColor
		{
			vec2 position;
			//vec4 color;

			VertexColor(){}
			VertexColor(vec2 pos)
				: position(pos)
			{
			}
		};

		struct VertexTexture
		{
			vec4 position;
			vec4 texCoord;
		};
	}
}