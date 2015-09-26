#pragma once
#include "engine/platform/inc_boost.h"
#include "engine/util/util.h"
#include <map>
#include <list>
#include <vector>

namespace Engine
{
	namespace Part
	{
		class IPart;
		typedef std::list<IPart*> PartList;
		typedef std::list<IPart*>::iterator PartIter;
		typedef std::map<int, PartList> PartPriorityMap;
		typedef std::map<int, PartList>::iterator PartPriorityIter;

		typedef Util::ulonglong					PartTypeKey;
		typedef std::function<IPart*(void)>		PartFunctor;

		enum EPriorities
		{
			ZERO = 0,
			SCALE = 1 << 8,
			IMG = 2 << 8,
			INPUT = 3 << 8,
			PHYSICS = 4 << 8,
			APP = 5 << 8
		};
	}

	namespace Object
	{
		class CObject;
		class CGroup;
		typedef std::vector<CObject*> ObjectList;
		typedef std::vector<CObject*>::iterator ObjectIter;
	}
}