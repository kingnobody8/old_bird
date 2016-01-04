#include "shader.h"

#include "shader/shader_color_polygon.h"

namespace engine
{
	namespace render
	{
		void RegisterShaders()
		{
			REGISTER_SHADER(ShaderColorPolygon);
		}
	}
}