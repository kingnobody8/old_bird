#include "engine/component/object.h"
#include "engine/component/part.h"

namespace Engine
{
	namespace Object
	{
		template<typename type>
		type* CObject::FindPart() const
		{
			return static_cast<type*>(this->FindPart(type::Type));
		}
	}
}