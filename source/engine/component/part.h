#pragma once
#include "component/component.h"

namespace engine
{
	namespace component
	{
		class IPart
		{
			//Internal
		private:
			static std::map<std::string, PartFunctor>& GetNameMap();// s_partNameMap;
			static std::map<PartTypeKey, PartFunctor>& GetKeyMap();// s_partKeyMap;
			static PartPriorityMap	s_vUpdateMap;
			static PartList			s_vPendingPartList;
			void DisableUpdateImmediate(void);

		protected: //TODO see if this has to be protected, try to make it private
			static int s_nextPartTypeId;

			//External
		public:
			static void UpdateParts(const util::Time& dt);
			static void CleanParts(void);
			static IPart* CreatePart(const std::string& type);
			static IPart* CreatePart(const int type);
			static PartTypeKey RegisterPart(const std::string& typeName, PartFunctor func);

			static const int Type = 0;
			virtual int GetType() const { return Type; }
			virtual const char* GetTypeName() const { return "IPart"; }

		private:
			bool				m_bUpdateEnabled;
			bool				m_bPendingUpdate;
			u16					m_usPartIndex;

			//------------------------------------------------------------------------------

		protected:
			CObject*			m_pOwner;
			ulong				m_priority;

		public:
			IPart();
			virtual ~IPart();
			IPart(const IPart& that);						//Copy Ctor
			IPart& operator= (const IPart& that);			//Assignment Operator

			virtual void Init(void){};
			virtual void Exit(void){};
			virtual void Reset(void);

			virtual void Copy(IPart* const that);
			virtual IPart* Clone(void);

			virtual void OnAttach(void){};					//NOTE there is no OnDetach because a part is only removed when it is destroyed, therefore Exit is called

			virtual void Destroy(void);
			virtual void Update(const util::Time& dt) {};

			void EnableUpdate(void);
			void DisableUpdate(void);

			virtual void LoadJson(const util::JSON& json){};
			//virtual rapidjson::Document SaveJson(void) const; //TODO

			virtual void OnMatrixChanged(void){};
			virtual void OnVisibilityChanged(const bool visible){};

			virtual util::shape::AABB CalcAABB(void) { return util::shape::AABB::INVALID_AABB; }

			//Gets
			inline CObject*				GetOwner(void) const { return this->m_pOwner; }
			inline const ulong&			GetPriority(void) const { return this->m_priority; }
			inline const bool			GetUpdateEnabled(void) const { return this->m_bUpdateEnabled; }
			virtual inline const bool	IsRenderable(void) const { return false; }
			//Sets
			virtual inline void			SetOwner(CObject* const pOwner) { this->m_pOwner = pOwner; }

			//PRE PROCESSOR
#define DECLARE_PART_TYPE_INFO(CLASS)																\
			typedef CLASS class_t;																	\
			static const int Type;																	\
			virtual inline int GetType() const { return Type; }										\
			virtual const char* GetTypeName() const;
#define DEFINE_PART_TYPE_INFO(CLASS)																\
			const int CLASS::Type = component::IPart::RegisterPart(#CLASS, util::CNewType<CLASS>());		\
			const char* CLASS::GetTypeName() const { return #CLASS; }
		};
	}
}
