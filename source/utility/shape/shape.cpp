#include "shape.h"
#include "ray.h"
#include "segment.h"
#include "macro.h"
#include <assert.h>
using namespace util::math;

namespace util
{
	namespace shape
	{
		IShape::~IShape(void)
		{
			/*LEAVE EMPTY - ABC*/
		}

		bool IShape::Intersect(const IShape* const test) const
		{
			__not_implemented(IShape::Intersect);//put in switch statement or function ptrs here to do intersection between the different types of shapes
			return false;
		}
	}
}