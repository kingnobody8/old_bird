#include "render_layer.h"
#include "render_node.h"
#include "camera.h"
#include <algorithm>

namespace engine
{
	namespace render
	{
		bool SortNodeFunc(IRenderNode* lhs, IRenderNode* rhs)
		{
			return lhs->GetZed() < rhs->GetZed();
		}

		bool SortLayerFunc(const CRenderLayer::Desc& lhs, const CRenderLayer::Desc& rhs)
		{
			return lhs.m_sort_rank > rhs.m_sort_rank;
		}

		STATIC std::list<CRenderLayer::Desc> CRenderLayer::s_layers;

		STATIC CRenderLayer* CRenderLayer::FindLayer(const std::string& szLayer)
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
			{
				if ((*iter).m_name == szLayer)
				{
					return (*iter).m_pLayer;
				}
			}
			return null;
		}

		STATIC CRenderLayer* CRenderLayer::CreateLayer(const std::string& name, const int& sort_rank, CCamera* const pCam)
		{
			assert(!name.empty() || pCam != null);

			CRenderLayer::Desc desc;
			desc.m_name = name;
			desc.m_pLayer = new CRenderLayer(name, pCam);
			desc.m_sort_rank = sort_rank;
			s_layers.push_back(desc);
			s_layers.sort(SortLayerFunc);
			return desc.m_pLayer;
		}

		STATIC void CRenderLayer::DestroyLayers()
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				delete (*iter).m_pLayer;
			s_layers.clear();
		}

		STATIC void CRenderLayer::RenderAllLayers(SDL_Renderer* pRen)
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				(*iter).m_pLayer->DoRender(pRen);
		}

		CRenderLayer::CRenderLayer(void)
			: m_pCamera(null)
		{
		}

		CRenderLayer::CRenderLayer(const std::string& name, CCamera* const pCam)
			: m_pCamera(pCam)
			, m_name(name)
		{
		}

		void CRenderLayer::Cull(const util::shape::AABB& view)
		{
			//Copy the real list to the temp list
			this->m_vCulledNodes.clear();
			this->m_vCulledNodes = this->m_vNodes;

			__todo()//TODO actually cull the nodes

			this->m_vNodes.sort(SortNodeFunc);
		}

		void CRenderLayer::DoRender(SDL_Renderer* pRen)
		{
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(pRen, &logical_size.w, &logical_size.h);
			util::math::vec2 half_dims(logical_size.x * 0.5f, logical_size.y * 0.5f);
			const util::shape::AABB view = m_pCamera->CalcViewAabb(half_dims);
			
			Cull(view);

			__todo()//remove this when you don't want to see the camera box anymore
			CRenderNodeRect cam_box;
			cam_box.SetAABB(m_pCamera->CalcViewAabb(half_dims));
			cam_box.SetFill(false);
			cam_box(pRen, util::math::Matrix2D()); //this is incorrect and should use the actual camera matrix


			__todo() //replace this with the nodes found after the cull
				//also use the actuall camera matrix

			util::math::Matrix2D inv_cam = util::math::Matrix2D::Matrix_Inverse(m_pCamera->GetMatrix());
			for (auto iter = m_vCulledNodes.begin(); iter != m_vCulledNodes.end(); ++iter)
			{
				(*iter)->operator()(pRen, inv_cam);
			}
		}

		void CRenderLayer::RegisterNode(IRenderNode* pNode)
		{
			assert(pNode);
			auto find = std::find(this->m_vNodes.begin(), this->m_vNodes.end(), pNode);
			if (find == this->m_vNodes.end())
			{
				this->m_vNodes.push_back(pNode);
				this->m_vNodes.sort(SortNodeFunc);
			}
		}

		void CRenderLayer::UnregisterNode(IRenderNode* pNode)
		{
			assert(pNode);
			this->m_vNodes.remove(pNode);
		}

		void CRenderLayer::ClearAllNodes(void)
		{
			this->m_vCulledNodes.clear();
			this->m_vNodes.clear();
		}
	}
}