#include "engine/render/render_layer.h"
#include "engine/render/render_node.h"
#include "engine/render/camera.h"
#include "engine/math/math.h"
#include <algorithm>

using namespace Engine::Math;

namespace Engine
{
	namespace Render
	{
		bool SortFunc(IRenderNode* lhs, IRenderNode* rhs)
		{
			return lhs->GetZed() < rhs->GetZed();
		}

		CRenderLayer::CRenderLayer(void)
			: m_pCamera(null)
		{
		}

		CRenderLayer::CRenderLayer(CCamera* pCam)
			: m_pCamera(pCam)
		{
		}

		void CRenderLayer::Cull(void)
		{
			//Copy the real list to the temp list
			this->m_vCulledNodes.clear();
			this->m_vCulledNodes = this->m_vNodes;

			//TODO actually cull the nodes
		}

		void CRenderLayer::DrawLayer(SDL_Renderer* pRen)
		{
			//Z Sort
			this->m_vNodes.sort(SortFunc);

			//Camera cull
			this->Cull();

			//Draw
			CMatrix2D inv_cam = Math::Matrix_Inverse(m_pCamera->GetMatrix());
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