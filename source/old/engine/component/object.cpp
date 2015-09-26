#include "engine/component/object.h"
#include "engine/component/part.h"
#include "engine/script/render_part.h"
#include <stack>
#include <queue>
using namespace Engine::Util;
using namespace Engine::Math;

namespace Engine
{
	namespace Object
	{
		/*------------------------------------------------------------------*/
		/*								COBJECT								*/
		/*------------------------------------------------------------------*/
		CObject::CObject(void)
			: m_pParent(null)
			, m_fZed(0.0f)
			, m_bVisible(VisibilityMask::SELF | VisibilityMask::PARENT)
		{
		}
		/*virtual*/ CObject::~CObject(void)
		{
		}
		CObject::CObject(const CObject& that)
		{
			*this = that;
		}
		CObject& CObject::operator=	(const CObject& that)
		{
			//Check for self assignment
			if (this == &that)
				return *this;

			assert(this->IsGroup() == that.IsGroup() && "Can't copy groups to objects and vice versa");

			//Uninitialize all current data
			this->Exit();

			//Fill out members
			this->m_szName = that.m_szName;
			this->m_pParent = null;
			this->m_cMatLocal = that.m_cMatLocal;
			this->m_fZed = that.m_fZed;
			this->m_bVisible = that.m_bVisible;

			ulong size = that.m_vParts.size();
			this->m_vParts.reserve(size);
			for (ulong i = 0; i < size; ++i)
			{
				this->AttachPart(that.m_vParts[i]->Clone());
			}
			return *this;
		}

		//Interface
		/*virtual*/ void CObject::Init(void)
		{
			std::list<Part::IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Init();
			}
		}
		/*virtual*/ void CObject::Exit(void)
		{
			std::list<Part::IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Exit();
			}
		}
		/*virtual*/ void CObject::Reset(void)
		{
			this->Exit();
			this->Init();
		}

		/*virtual*/ void CObject::Copy(const CObject& that)
		{
			*this = that;
		}
		/*virtual*/ void CObject::Copy(const CGroup& that)
		{
			assert(false && "Shouldn't ever get here, objects can't copy groups");
		}
		/*virtual*/ CObject* CObject::Clone(void)
		{
			CObject* ret = new CObject();
			ret->Copy(*this);
			return ret;
		}

		/*virtual*/ void CObject::Drop(void)
		{
			assert(m_pParent);
			this->m_pParent->RemoveObject(this);
		}
		/*virtual*/ void CObject::Destroy(void)
		{
			std::list<Part::IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Destroy();
			}
			delete this;
		}

		/*virtual*/ void CObject::AttachPart(Part::IPart* const pPart)
		{
			assert(pPart);
			pPart->SetOwner(this);
			this->m_vParts.push_back(pPart);
			pPart->OnAttach();
		}
		/*virtual*/ bool CObject::DetachPart(Part::IPart* const pPart)
		{
			assert(pPart);
			int size = this->m_vParts.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vParts[i] == pPart)
				{
					this->m_vParts[i]->Destroy();
					this->m_vParts.erase(this->m_vParts.begin() + i);
					return true;
				}
			}
			return false;
		}

		/*virtual*/ void CObject::LoadJson(const Json::JSON& json)
		{
			//Object
			this->m_szName = json["name"].GetString();
			this->m_fZed = json["matrix"]["position"]["z"].GetDouble();
			this->SetLocalMatrix(json["matrix"].GetMatrix());
			const std::vector<Json::JSON> parts = json["parts"].GetArray<Json::JSON>();
			//Parts
			for (ulong i = 0; i < parts.size(); ++i)
			{
				std::string type = parts[i]["type"].GetString();
				Part::IPart* pPart = Part::IPart::CreatePart(type);
				pPart->LoadJson(parts[i]);
				this->AttachPart(pPart);
			}
		}

		//Calc
		const Math::CMatrix2D CObject::CalcWorldMatrix(void) const
		{
			//TODO i don't think we need to run through this twice (up then down)
			//we should be able to run it up and get the final matrix
			std::vector<CMatrix2D> vec;
			vec.push_back(this->m_cMatLocal);
			TraverseAncestors(this, [&vec](CObject* pObj)
			{
				vec.push_back(pObj->GetLocalMatrix());
			});

			CMatrix2D world;
			for (int i = vec.size() - 1; i >= 0; --i)
			{
				world *= vec[i];
			}

			return world;
		}
		const float CObject::CalcWorldZed(void) const
		{
			float ret = this->m_fZed;
			TraverseAncestors(this, [&ret](CObject* pObj)
			{
				ret += pObj->GetLocalZed();
			});
			return ret;
		}
		const Math::AABB CObject::CalcAabb(void) const
		{
			//TODO this doesn't take into account the position of the aabb, we need the points in world space

			AABB aabb;
			aabb.MakeLimits();
			std::list<Part::IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				if ((*iter)->IsRenderable())
				{
					Script::IRenderPart* pRenderPart = static_cast<Script::IRenderPart*>((*iter));
					aabb.Extend(pRenderPart->GetAABB());
				}
			}

			return aabb;
		}

		//Sets
		void CObject::SetParent(CGroup* const pParent)
		{
			if (this->m_pParent)
				this->Drop();
			this->m_pParent = pParent;
		}
		void CObject::SetLocalMatrix(const Math::CMatrix2D& mat)
		{
			m_cMatLocal = mat;
			OnMatrixChanged();
		}
		void CObject::SetWorldMatrix(const Math::CMatrix2D& mat)
		{
			CMatrix2D temp = mat;
			if (this->m_pParent)
			{
				temp = this->m_pParent->CalcWorldMatrix() * Math::Matrix_Inverse(mat);
			}
			this->SetLocalMatrix(temp);
		}

		void CObject::SetLocalPosX(const float& x)
		{
			this->m_cMatLocal.SetPositionX(x);
			OnMatrixChanged();
		}
		void CObject::SetLocalPosY(const float& y)
		{
			this->m_cMatLocal.SetPositionY(y);
			OnMatrixChanged();
		}
		void CObject::SetLocalPosXY(const float& x, const float& y)
		{
			this->m_cMatLocal.SetPosition(x, y);
			OnMatrixChanged();
		}
		void CObject::SetLocalScale(const float& x, const float& y)
		{
			this->m_cMatLocal.SetScale(x, y);
			OnMatrixChanged();
		}
		void CObject::SetLocalRotationZ(const float& rot)
		{
			this->m_cMatLocal.SetRotationZ(rot);
			OnMatrixChanged();
		}

		void CObject::SetWorldPosX(const float& x)
		{
			if (m_pParent)
			{
				const CMatrix2D& wmat = this->m_pParent->CalcWorldMatrix();
				vec2 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				float pos_x = x / wmat.GetScale().x - wpos.x;
				this->SetLocalPosX(pos_x);
			}
			else
				this->SetLocalPosX(x);
		}
		void CObject::SetWorldPosY(const float& y)
		{
			if (m_pParent)
			{
				const CMatrix2D& wmat = this->m_pParent->CalcWorldMatrix();
				vec2 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				float pos_y = y / wmat.GetScale().y - wpos.y;
				this->SetLocalPosY(pos_y);
			}
			else
				this->SetLocalPosY(y);
		}
		void CObject::SetWorldPosXY(const float& x, const float& y)
		{
			if (m_pParent)
			{
				const CMatrix2D& wmat = this->m_pParent->CalcWorldMatrix();
				vec2 wpos = this->m_pParent->CalcWorldMatrix().GetPosition();
				vec2 pos = vec2(x,y) / wmat.GetScale() - wpos;
				this->SetLocalPosXY(pos);
			}
			else
			{
				this->SetLocalPosXY(x, y);
			}
		}
		void CObject::SetWorldScale(const float& x, const float& y)
		{
			if (m_pParent)
			{
				vec2 wscale = this->m_pParent->CalcWorldMatrix().GetScale();
				this->SetLocalScale(x - wscale.x, y - wscale.y);
			}
			else
			{
				this->SetLocalScale(x, y);
			}
		}
		void CObject::SetWorldRotationZ(const float& rot)
		{
			if (m_pParent)
				this->SetLocalRotationZ(rot - this->m_pParent->CalcWorldMatrix().GetRotationZ());
			else
				this->SetLocalRotationZ(rot);
		}

		/*virtual*/ void CObject::SetVisible(const bool bVis)
		{
			m_bVisible.Bool(VisibilityMask::SELF, bVis);
			OnVisibilityChanged(bVis);
		}

		//Find
		Part::IPart* CObject::FindPart(const std::string& szType) const
		{
			int size = this->m_vParts.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vParts[i]->GetTypeName() == szType)
				{
					return this->m_vParts[i];
				}
			}
			return null;
		}
		Part::IPart* CObject::FindPart(const Part::PartTypeKey& key) const
		{
			int size = this->m_vParts.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vParts[i]->GetType() == key)
				{
					return this->m_vParts[i];
				}
			}
			return null;
		}

		//Protected
		void CObject::OnMatrixChanged(void)
		{
			std::list<Part::IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnMatrixChanged();
			}
		}
		void CObject::OnVisibilityChanged(const bool bVisible)
		{
			std::list<Part::IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnVisibilityChanged(GetWorldVisible());
			}
		}
		void CObject::OnParentVisibilityChanged(const bool bVisible)
		{
			this->m_bVisible.Bool(VisibilityMask::PARENT, bVisible);
			std::list<Part::IPart*> list = GetPartList(this);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->OnVisibilityChanged(GetWorldVisible());
			}
		}

		/*------------------------------------------------------------------*/
		/*								CGROUP								*/
		/*------------------------------------------------------------------*/
		CGroup::CGroup(void)
		{
		}
		/*virtual*/ CGroup::~CGroup(void)
		{
		}
		CGroup::CGroup(const CGroup& that)
		{
			*this = that;
		}
		CGroup& CGroup::operator = (const CGroup& that)
		{
			CObject::operator=(that);

			ulong size = that.m_vChildren.size();
			this->m_vChildren.reserve(size);
			for (ulong i = 0; i < size; ++i)
			{
				this->AppendObject(that.m_vChildren[i]->Clone());
			}
			return *this;
		}

		//Interface
		/*virtual*/ void CGroup::Copy(const CObject& that)
		{
			assert(false && "Shouldn't ever get here, groups can't copy objects");
		}
		/*virtual*/ void CGroup::Copy(const CGroup& that)
		{
			*this = that;
		}
		/*virtual*/ CObject* CGroup::Clone(void)
		{
			CGroup* ret = new CGroup();
			ret->Copy(*this);
			return ret;
		}

		/*virtual*/ void CGroup::AppendObject(CObject* const pObj)
		{
			assert(pObj);
			this->m_vChildren.push_back(pObj);
			pObj->SetParent(this);
		}
		/*virtual*/ bool CGroup::RemoveObject(CObject* const pObj)
		{
			assert(pObj);
			ulong size = this->m_vChildren.size();
			for (ulong i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i] == pObj)
				{
					this->m_vChildren.erase(this->m_vChildren.begin() + i);
					return true;
				}
			}
			return false;
		}

		/*virtual*/ void CGroup::Destroy(void)
		{
			std::list<Part::IPart*> list = GetPartList(this, true);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Destroy();
			}

			std::list<CObject*> children;
			TraverseChildren(this, [&children](CObject* pChild)
			{
				children.push_back(pChild);
			});
			for (auto iter = children.begin(); iter != children.end(); ++iter)
			{
				delete (*iter);
			}

			delete this;
		}

		/*virtual*/ void CGroup::LoadJson(const Json::JSON& json)
		{
			//Object
			CObject::LoadJson(json);
			//Children
			const std::vector<Json::JSON> children = json["children"].GetArray<Json::JSON>();
			for (ulong i = 0; i < children.size(); ++i)
			{
				CObject* pObj = LoadObjectFromJson(children[i]);
				this->AppendObject(pObj);
			}
		}

		//Sets
		/*virtual*/ void CGroup::SetVisible(const bool bVis)
		{
			m_bVisible.Bool(VisibilityMask::SELF, bVis);
			TraverseChildren(this, [&bVis](CObject* pChild)
			{
				pChild->OnParentVisibilityChanged(bVis);
			});
			OnVisibilityChanged(bVis);
		}

		//Finds
		CObject* CGroup::FindObject(const std::string& szName
			, const int& depth) const
		{
			//TODO once we get a depth first traversal func we should use that hereC

			int size = this->m_vChildren.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->GetName() == szName)
				{
					return this->m_vChildren[i];
				}
			}

			//If we get here then search our children's children for the object
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(this->m_vChildren[i]);
					CObject* ret = pGroup->FindObject(szName);
					if (ret)
						return ret;
				}
			}
			return null;
		}

		CGroup* CGroup::FindGroup(const std::string& szName
			, const int& depth) const
		{
			//TODO once we get a depth first traversal func we should use that here

			int size = this->m_vChildren.size();
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->GetName() == szName && this->m_vChildren[i]->IsGroup())
				{
					return dynamic_cast<CGroup*>(this->m_vChildren[i]);
				}
			}

			//If we get here then search our children's children for the object
			for (int i = 0; i < size; ++i)
			{
				if (this->m_vChildren[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(this->m_vChildren[i]);
					CGroup* ret = pGroup->FindGroup(szName);
					if (ret)
						return ret;
				}
			}
			return null;
		}

		/*------------------------------------------------------------------*/
		/*								FUNCTIONS							*/
		/*------------------------------------------------------------------*/
		void Update(Object::CObject* const pObj, const Time& dt)
		{
			assert(pObj);
			std::list<Part::IPart*> list = GetPartList(pObj);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				(*iter)->Update(dt);
			}
		}
		
		const bool IsRenderable(const Object::CObject* const pObj)
		{
			assert(pObj);
			std::list<Part::IPart*> list = GetPartList(pObj);
			for (auto iter = list.begin(); iter != list.end(); ++iter)
			{
				if ((*iter)->IsRenderable())
					return true;
			}
			return false;
		}

		CObject* LoadObjectFromJson(const Json::JSON& json)
		{
			CObject* ret = null;

			//Check is group or object
			std::vector<Json::JSON> val = json["children"].GetArray<Json::JSON>();
			if (val.empty())
				ret = new CObject();
			else
				ret = new CGroup();

			ret->LoadJson(json);

			return ret;
		}

		const Part::PartList GetPartList(const Object::CObject* const pObj, const bool& reverse_priority)
		{
			assert(pObj);
			Part::PartList list;

			std::vector<Part::IPart*> vec = pObj->GetParts();
			for (ulong i = 0; i < vec.size(); ++i)
			{
				list.push_back(vec[i]);
			}

			if (pObj->IsGroup())
			{
				const CGroup* const pGroup = static_cast<const CGroup* const>(pObj);
				TraverseChildren(pGroup, [&list](Object::CObject* pObj)
				{
					std::vector<Part::IPart*> vec = pObj->GetParts();
					for (ulong i = 0; i < vec.size(); ++i)
					{
						list.push_back(vec[i]);
					}
				});
			}

			//Type of sorting function
			std::function<bool(Part::IPart* lhs, Part::IPart* rhs)> func;
			if (reverse_priority)
			{
				func = [](Part::IPart* lhs, Part::IPart* rhs)
				{
					return lhs->GetPriority() > rhs->GetPriority();
				};
			}
			else
			{
				func = [](Part::IPart* lhs, Part::IPart* rhs)
				{
					return lhs->GetPriority() < rhs->GetPriority();
				};
			}

			list.sort(func);

			return list;
		}

		void TraverseAncestors(const CObject* const that
			, std::function<void(CObject*const)> func
			, EDirection eDirection)
		{
			assert(that);
			switch (eDirection)
			{
			case EDirection::UP:
			{
				CObject* pParent = that->GetParent();
				if (pParent)
				{
					func(pParent);
					TraverseAncestors(pParent, func, eDirection);
				}
				break;
			}
			case EDirection::DOWN:
			{
				std::vector<CObject*> parents;
				CObject* pParent = that->GetParent();
				while (pParent)
				{
					parents.push_back(pParent);
					pParent = pParent->GetParent();
				}
				for (int i = parents.size() - 1; i >= 0; --i)
				{
					func(parents[i]);
				}
				break;
			}
			default:
				assert(false);
			}
			
		}

		void TraverseChildren(const CGroup* const that
			, std::function<void(CObject*const)> func
			, int depth)
		{
			assert(that);
			std::vector<CObject*> children = that->GetChildren();
			int size = children.size();
			for (int i = 0; i < size; ++i)
			{
				func(children[i]);
			}
			
			if (depth == 0)
				return;
			
			for (int i = 0; i < size; ++i)
			{
				if (children[i]->IsGroup())
				{
					CGroup* pGroup = static_cast<CGroup*>(children[i]);
					TraverseChildren(pGroup, func, depth - 1);
				}
			}
		}

		void TraverseChildrenBreak(CGroup* const that
			, std::function<bool(CObject*const)> func
			, const int& depth)
		{
			assert(false);
			//TODO
		}


		/*typedef std::pair<CObject*, bool> Node;
		void TraverseBreadthFirst(CObject* root, std::function<void(CObject*)> func)
		{
			assert(root);

			std::queue<Node> queue;
			queue.push(Node(root,true));
			func(root);

			while (!queue.empty())
			{
				Node node = queue.front();
				queue.pop();
				if (node.first->IsGroup())
				{
					CGroup* pGroup = reinterpret_cast<CGroup*>(node.first);
					std::vector<CObject*> children = pGroup->GetChildren();
					for (int i = 0, size = children.size(); i < size; ++i)
					{
						func(children[i]);
						queue.push(Node(children[i], true));
					}
				}
			}
		}
		void TraverseDepthFirst(CObject* root, std::function<void(CObject*)> func)
		{
			assert(root);

			std::stack<Node> stack;
			stack.push(Node(root, false));
			while (!stack.empty())
			{
				Node node = stack.top();
				stack.pop();
				if (!node.second)
				{
					func(node.first);
					node.second = true;
					if (node.first->IsGroup())
					{
						CGroup* pGroup = reinterpret_cast<CGroup*>(node.first);
						std::vector<CObject*> children = pGroup->GetChildren();
						for (int i = 0, size = children.size(); i < size; ++i)
						{
							stack.push(Node(children[i], false));
						}
					}
				}
			}
		}
*/
	}
}
