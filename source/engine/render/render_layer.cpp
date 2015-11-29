#include "render_layer.h"
#include "node/render_node.h"
#include "camera.h"
#include "renderer.h"
#include <algorithm>

namespace engine
{
	namespace render
	{
		bool SortNodeFunc(IRenderNode* lhs, IRenderNode* rhs)
		{
			return lhs->GetZed() < rhs->GetZed();
		}

		bool SortLayerFunc(CRenderLayer* lhs, CRenderLayer* rhs)
		{
			return lhs->GetSortRank() > rhs->GetSortRank();
		}

		STATIC std::list<CRenderLayer*> CRenderLayer::s_layers;

		STATIC CRenderLayer* CRenderLayer::FindLayer(const std::string& szLayer)
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
			{
				if ((*iter)->GetName() == szLayer)
				{
					return (*iter);
				}
			}
			return null;
		}

		STATIC CRenderLayer* CRenderLayer::CreateLayer(const std::string& name, const int& sort_rank, CCamera* const pCam)
		{
			assert(!name.empty() || pCam != null);

			CRenderLayer* layer = new CRenderLayer(name, sort_rank, pCam);
			s_layers.push_back(layer);
			s_layers.sort(SortLayerFunc);
			return layer;
		}

		STATIC void CRenderLayer::DestroyLayers()
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				delete (*iter);
			s_layers.clear();
		}

		STATIC void CRenderLayer::RenderAllLayers(SDL_Renderer* pRen)
		{
			for (auto iter = s_layers.begin(); iter != s_layers.end(); ++iter)
				(*iter)->DoRender(pRen);
		}

		CRenderLayer::CRenderLayer(void)
			: m_pCamera(null)
			, m_sort_rank(0)
		{
		}

		CRenderLayer::CRenderLayer(const std::string& name, const int& sort_rank, CCamera* const pCam)
			: m_pCamera(pCam)
			, m_name(name)
			, m_sort_rank(sort_rank)
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

			//__todo()//remove this when you don't want to see the camera box anymore
			//CRenderNodeRect cam_box;
			//cam_box.SetAABB(m_pCamera->CalcViewAabb(half_dims));
			//cam_box.SetFill(false);
			//cam_box(pRen, util::math::Matrix2D()); //this is incorrect and should use the actual camera matrix


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

		const util::math::vec2 CRenderLayer::ConvertPointFromScreenToWorld(util::math::vec2 m_screen_point)
		{
			//get screen info
			util::math::Type2<int> logical_size;
			SDL_GetRendererOutputSize(render::GetSdlRenderer(), &logical_size.w, &logical_size.h);
			util::math::vec2 origin(logical_size.x * 0.5f, logical_size.y * 0.5f);

			m_screen_point.x = m_screen_point.x - origin.x;
			m_screen_point.y = origin.y - m_screen_point.y;

			return util::math::Matrix2D::Vector_Matrix_Multiply(m_screen_point, m_pCamera->GetMatrix());
		}

		const util::math::vec2 CRenderLayer::ConvertPointFromWorldToScreen(util::math::vec2 m_world_point)
		{
			return util::math::Matrix2D::Vector_Matrix_Multiply(m_world_point, util::math::Matrix2D::Matrix_Inverse(m_pCamera->GetMatrix()));
		}
	}
}