#include "engine/component/part.h"
#include "engine/component/object.h"

namespace Engine
{
	namespace Part
	{
		static const u16 INVALID_PART_INDEX = 0xFFFF;

		/*static*/ std::map<std::string, PartFunctor>& IPart::GetNameMap()
		{
			static std::map<std::string, PartFunctor> impl;
			return impl;
		}
		/*static*/ std::map<PartTypeKey, PartFunctor>& IPart::GetKeyMap()
		{
			static std::map<PartTypeKey, PartFunctor> impl;
			return impl;
		}
		/*static*/ PartPriorityMap IPart::s_vUpdateMap;
		/*static*/ PartList IPart::s_vPendingPartList;
		/*static*/ int IPart::s_nextPartTypeId = 0;

		/*static*/ void IPart::UpdateParts(float dt)
		{
			CleanParts();
			for (PartPriorityIter map_iter = s_vUpdateMap.begin(); map_iter != s_vUpdateMap.end(); ++map_iter)
			{
				PartList& list = (*map_iter).second;
				for (PartIter part_iter = list.begin(); part_iter != list.end(); ++part_iter)
				{
					if ((*part_iter)->m_bUpdateEnabled)
						(*part_iter)->Update(dt);
				}
			}
		}
		/*static*/ void IPart::CleanParts(void)
		{
			for (PartIter iter = s_vPendingPartList.begin(); iter != s_vPendingPartList.end(); ++iter)
			{
				IPart* part = (*iter);
				part->m_bPendingUpdate = false;
				PartList& list = s_vUpdateMap[part->m_priority];
				if (part->m_bUpdateEnabled) //Enable
				{
					// Guard against enabling multiple times
					if (part->m_usPartIndex == INVALID_PART_INDEX)
					{
						part->m_usPartIndex = (u16)list.size();
						list.push_back(part);
					}
				}
				else //Disable
				{
					if (part->m_usPartIndex != INVALID_PART_INDEX)
					{
						if (!list.empty())
						{
							PartIter ours = list.begin();
							std::advance(ours, part->m_usPartIndex);
							PartIter back = list.end();
							std::advance(back, -1);
							std::swap(ours, back);
							list.pop_back();
						}
						//Invalidate the index
						part->m_usPartIndex = INVALID_PART_INDEX;
					}
				}
			}
			s_vPendingPartList.clear();
		}
		/*static*/ IPart* IPart::CreatePart(const std::string& type)
		{
			auto find = IPart::GetNameMap().find(type);
			assert(find != IPart::GetNameMap().end());
			return find->second();
		}
		/*static*/ IPart* IPart::CreatePart(const int type)
		{
			auto find = IPart::GetKeyMap().find(type);
			assert(find != IPart::GetKeyMap().end());
			return find->second();
		}
		/*static*/ PartTypeKey IPart::RegisterPart(const std::string& typeName, PartFunctor func)
		{
			int key = ++IPart::s_nextPartTypeId;
			IPart::GetNameMap()[typeName] = func;
			IPart::GetKeyMap()[key] = func;
			return key;
		}

		//------------------------------------------------------------------------------

		IPart::IPart(void)
			: m_pOwner(null)
			, m_priority(0)
			, m_bUpdateEnabled(false)
			, m_bPendingUpdate(false)
			, m_usPartIndex(INVALID_PART_INDEX)
		{
		}
		/*virtual*/ IPart::~IPart(void)
		{
			DisableUpdateImmediate();
		}
		IPart::IPart(const IPart& that)
		{
			*this = that;
		}
		IPart& IPart::operator= (const IPart& that)
		{
			//Check for self assignment
			if (this == &that)
				return *this;

			assert(this->GetType() == that.GetType() && "WE SHOULDN'T COPY OR CLONE A PART ONTO A DIFFERENT TYPE OF PART");
			m_pOwner = null;
			return *this;
		}

		/*virtual*/ void IPart::Reset(void)
		{
			this->Exit();
			this->Init();
		}

		/*virtual*/ void IPart::Destroy(void)
		{
			this->Exit();
			delete this;
		}

		/*virtual*/ void IPart::Copy(IPart* const that)
		{
			*this = *that;
		}
		/*virtual*/ IPart* IPart::Clone(void)
		{
			IPart* ret = Part::IPart::CreatePart(this->GetType());
			ret->Copy(this);
			return ret;
		}

		void IPart::EnableUpdate(void)
		{
			if (m_bUpdateEnabled)
				return;
			m_bUpdateEnabled = true;

			if (m_bPendingUpdate)
				return;
			m_bPendingUpdate = true;

			s_vPendingPartList.push_back(this);
		}
		void IPart::DisableUpdate(void)
		{
			if (!m_bUpdateEnabled)
				return;
			m_bUpdateEnabled = false;

			if (m_bPendingUpdate)
				return;
			m_bPendingUpdate = true;

			s_vPendingPartList.push_back(this);
		}
		void IPart::DisableUpdateImmediate(void)
		{
			if (this->m_usPartIndex != INVALID_PART_INDEX)
			{
				PartList& list = s_vUpdateMap[this->m_priority];
				if (!list.empty())
				{
					PartIter ours = list.begin();
					std::advance(ours, this->m_usPartIndex);
					PartIter back = list.end();
					std::advance(back, -1);
					std::swap(ours, back);
					list.pop_back();
				}
				//Invalidate the index
				this->m_usPartIndex = INVALID_PART_INDEX;
			}
		}
}
}
