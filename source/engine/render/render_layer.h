#pragma once
#include "platform/inc_sdl.h"
#include <list>
#include "macro.h"

namespace engine
{
	namespace render
	{
		//Forward Declare
		class IRenderNode;
		class CCamera;

		class CRenderLayer
		{
		public:
			struct Desc
			{
				CRenderLayer* m_pLayer;
				std::string m_name;
				int			m_sort_rank;
			};

		private:
			static std::list<CRenderLayer::Desc> s_layers;

		public:
			__todo() // i don't think we will need/want this
			//static const std::list<CRenderLayer*>& GetLayers() { return s_pLayers; }
			static CRenderLayer* FindLayer(const std::string& szLayer);
			static CRenderLayer* CreateLayer(const std::string& name, const int& sort_rank, CCamera* const pCam);
			static void DestroyLayers();
			static void RenderAllLayers(SDL_Renderer* pRen);

		private:
			std::list<IRenderNode*> m_vNodes;
			std::list<IRenderNode*> m_vCulledNodes;
			CCamera* m_pCamera;
			std::string m_name;

		private:
			void Cull(void);

			CRenderLayer();
			CRenderLayer(const std::string& name, CCamera* const pCam);

		public:
			void RegisterNode(IRenderNode* pNode);
			void UnregisterNode(IRenderNode* pNode);
			void ClearAllNodes(void);
			void DoRender(SDL_Renderer* pRen);
			inline CCamera* const GetCamera(void) const { return m_pCamera; }
			inline const std::string& GetName() const { return m_name; }
		};
	}
}