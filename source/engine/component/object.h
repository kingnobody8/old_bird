#pragma once
#include "component.h"

namespace engine
{
	namespace component
	{
		//forward declare
		class IPart;
		class CGroup;

		enum VisibilityMask
		{
			SELF = 0x01,
			PARENT = 0x02
		};

		class CObject
		{
			friend class CGroup;		//This is so that the CGroup class may access private and protected funcs and data for their children

			/*Data*/
		private:
		protected:
			std::string					m_szName;
			CGroup*						m_pParent;
			std::vector<IPart*>			m_vParts;
			util::math::Matrix2D		m_cMatLocal;
			float						m_fZed;
			util::Flag08				m_bVisible;
		public:

			/*Func*/
		private:
		protected:
		public:
			CObject(void);											//Ctor
			virtual ~CObject(void);									//Dtor
			CObject(const CObject& that);							//Copy Ctor
			CObject& operator=	(const CObject& that);				//Assignment Operator

			//Interface
			virtual void Init(void);								//Allocates memory and sets all members to a useable state
			virtual void Exit(void);								//Deallocates any memory that is being used and sets all members to a default 'unset' state. Ex -1 or NULL
			virtual void Reset(void);								//Calls Exit then Init

			virtual void Copy(const CObject& that);					//Copies all data and scripts from 'that' object onto 'this' object
			virtual void Copy(const CGroup& that);					//exists to assert
			virtual CObject* Clone(void);							//Returns a new object that is an exact copy of 'this' object (except the clone's parent is null)

			virtual void Drop(void);								//Tells 'this' object's parent to remove us from their children list (does not destroy the object)
			virtual void Destroy(void);								//Destroys all parts, before destroying itself

			virtual void AttachPart(IPart* const pPart);			//Attaches a part script to the 'this' object, CALL INIT YOURSELF
			virtual bool DetachPart(IPart* const pPart);			//Calls the destroy function for the script, then detaches it from the object, return true for success

			virtual void LoadJson(const util::JSON& json);
			virtual const util::JSON SaveJson(void) const;

			//Gets
			inline const std::string&				GetName(void) const { return this->m_szName; }
			inline CGroup*							GetParent(void) const { return this->m_pParent; }
			inline const std::vector<IPart*>&		GetParts(void) const { return this->m_vParts; }
			inline const util::math::Matrix2D&		GetLocalMatrix(void) const { return this->m_cMatLocal; }
			inline const float&						GetLocalZed(void) const { return this->m_fZed; }
			inline const bool						GetLocalVisible(void) const { return this->m_bVisible.Bit(VisibilityMask::SELF); }
			inline const bool						GetWorldVisible(void) const { return this->m_bVisible.Bit(VisibilityMask::SELF) && this->m_bVisible.Bit(VisibilityMask::PARENT); }

			inline const vec2						GetLocalPos(void) const { return this->m_cMatLocal.GetPosition(); }
			inline const vec2						GetLocalScale(void) const { return this->m_cMatLocal.GetScale(); }
			inline const float						GetLocalRotationZ(void) const { return this->m_cMatLocal.GetRotationZ(); }

			//Calcs
			const util::math::Matrix2D				CalcWorldMatrix(void) const;
			const float								CalcWorldZed(void) const;

			inline const vec2						CalcWorldPos(void) const { return this->CalcWorldMatrix().GetPosition(); }
			inline const vec2						CalcWorldScale(void) const { return this->CalcWorldMatrix().GetScale(); }
			inline const float						CalcWorldRotationZ(void) const { return this->CalcWorldMatrix().GetRotationZ(); }

			const util::shape::AABB					CalcAabb(void) const;

			virtual inline bool						IsGroup(void) const { return false; }

			//Sets
			inline void								SetName(const std::string& szName) { this->m_szName = szName; }
			void									SetParent(CGroup* const pParent);
			void									SetLocalMatrix(const util::math::Matrix2D& mat);
			void									SetWorldMatrix(const util::math::Matrix2D& mat);

			void									SetLocalPosX(const float& x);
			void									SetLocalPosY(const float& y);
			void									SetLocalPosXY(const float& x, const float& y);
			inline void								SetLocalPosXY(const vec2& pos) { this->SetLocalPosXY(pos.x, pos.y); }
			void									SetLocalScale(const float& x, const float& y);
			inline void								SetLocalScale(const vec2& scale) { this->SetLocalScale(scale.x, scale.y); }
			void									SetLocalRotationZ(const float& rot);

			void									SetWorldPosX(const float& x);
			void									SetWorldPosY(const float& y);
			void									SetWorldPosXY(const float& x, const float& y);
			inline void								SetWorldPosXY(const vec2& pos) { this->SetWorldPosXY(pos.x, pos.y); }
			void									SetWorldScale(const float& x, const float& y);
			inline void								SetWorldScale(const vec2& scale) { this->SetWorldScale(scale.x, scale.y); }
			void									SetWorldRotationZ(const float& rot);

			inline void								SetLocalZed(const float& fZed) { this->m_fZed = fZed; }
			virtual void							SetVisible(const bool bVis);

			//Finds
			template<typename type>
			type*									FindPart() const;
			IPart*									FindPart(const std::string& szType) const;
			IPart*									FindPart(const PartTypeKey& key) const;

		protected:
			void OnMatrixChanged(void);
			void OnVisibilityChanged(const bool bVisible);
			void OnParentVisibilityChanged(const bool bVisible);
		};

		/* Container for objects, which also can act like an object itself */
		class CGroup : public CObject
		{
			/*Data*/
		private:
		protected:
			std::vector<CObject*>		m_vChildren;
		public:

			/*Func*/
		private:
		protected:
		public:
			CGroup(void);										//Ctor
			virtual ~CGroup(void);								//Dtor
			CGroup(const CGroup& that);							//Copy Ctor
			CGroup& operator= (const CGroup& that);				//Assignment Operator

			//Interface
			virtual void Copy(const CObject& that);				//exists to assert
			virtual void Copy(const CGroup& that);				//Copies all data and scripts from 'that' object onto 'this' object
			virtual CObject* Clone(void);						//Returns a new object that is an exact copy of 'this' object (except it's parent is null)

			virtual void AppendObject(CObject* const pObj);		//Appends the 'pObj' to the children list
			virtual bool RemoveObject(CObject* const pObj);		//Removes an object from the children list, return true for success, CALL EXIT YOURSELF

			virtual void Destroy(void);								//Destroys all parts, before destroying itself

			virtual void LoadJson(const util::JSON& json);
			virtual const util::JSON SaveJson(void) const;

			//Gets
			inline std::vector<CObject*>			GetChildren(void) const { return this->m_vChildren; }
			virtual inline bool						IsGroup(void) const { return true; }

			//Sets
			virtual void							SetVisible(const bool bVis);

			//Finds
			CObject*						FindObject(const std::string& szName, const int& depth = -1) const;//Searches for an object in the children's list that has the same name, the first object to match is returned, searching will continue through through the specified depth
			CGroup*							FindGroup(const std::string& szName, const int& depth = -1) const;//Searches for a group object in the children's list that has the same name, the first group object to match is returned, searching will continue through through the specified depth
		};


		CObject*						LoadObjectFromJson(const util::JSON& json);
		void							Update(CObject* const pObj, const util::Time& dt);
		const bool						IsRenderable(const CObject* const pObj);
		const PartList					GetPartList(const CObject* const pObj, const bool& reverse_priority = false);
		void							TraverseAncestors(const CObject* const that, std::function<void(CObject*const)> func, const util::EDirection eDirection = util::EDirection::UP);//Traverses through each parent either from 'that' to the root or from the root to 'that'
		void							TraverseChildren(const CGroup* const that, std::function<void(CObject*const)> func, const int depth = -1); //Traverses through all children up to the specified depth, -1 means do all of them
		void							TraverseChildrenBreak(CGroup* const that, std::function<bool(CObject*const)> func, const int depth = -1); //Traverse just like the original function, but now the callback can break the loop by returning true

		//void TraversePushPopBreadth(CObject* const that, std::function<void(CObject*const)> push, std::function<void(CObject*)> pop);
		//void TraversePushPopDepth(CObject* const that, std::function<void(CObject*const)> push, std::function<void(CObject*)> pop);
		//void TraverseBreadthFirst(CObject* root, std::function<void(CObject*)> func);//Visits all children calling the function on it, function has no break out
		//void TraverseDepthFirst(CObject* root, std::function<void(CObject*)> func);//Visits all children calling the function on it, function has no break out
}
}