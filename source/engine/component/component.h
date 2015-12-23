#pragma once
#include "func.h"
#include "types.h"
#include "math/type2.h"
#include "math/type3.h"
#include "math/type4.h"
#include "math/matrix_2d.h"
#include "box2d/Box2D.h"
#include "json.h"
#include "bit_flag.h"
#include "enums.h"
#include <map>
#include <list>
#include <vector>

namespace engine
{
	namespace component
	{
		class IPart;
		typedef std::list<IPart*> PartList;
		typedef std::list<IPart*>::iterator PartIter;
		typedef std::map<int, PartList> PartPriorityMap;
		typedef std::map<int, PartList>::iterator PartPriorityIter;

		typedef ulonglong						PartTypeKey;
		typedef std::function<IPart*(void)>		PartFunctor;

		class CObject;
		class CGroup;
		typedef std::vector<CObject*> ObjectList;
		typedef std::vector<CObject*>::iterator ObjectIter;

		enum EFlag
		{
			ACTIVE = 0x01,
			SELF_VISIBLE = 0x02,	//part doesn't use this
			PARENT_VISIBLE = 0x04,	//part doesn't use this
			STARTED = 0x08,
			STOPPED = 0x10,
			NUKED = 0x20
		};
	}
}