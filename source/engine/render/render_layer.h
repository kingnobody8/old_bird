#pragma once
#include "sdl/include/SDL.h"
#include <list>
#include "macro.h"
#include "shape/aabb.h"
#include <string>

namespace engine
{
	namespace render
	{
		//Forward Declare
		class IRenderNode;
		class CCamera;

		class CRenderLayer
		{
		private:
			static std::list<CRenderLayer*> s_layers;

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
			int m_sort_rank;

		private:
			void Cull(const util::shape::AABB& view);

			CRenderLayer();
			CRenderLayer(const std::string& name, const int& sort_rank, CCamera* const pCam);

		public:
			void RegisterNode(IRenderNode* pNode);
			void UnregisterNode(IRenderNode* pNode);
			void ClearAllNodes(void);
			void DoRender(SDL_Renderer* pRen);
			inline CCamera* const GetCamera(void) const { return m_pCamera; }
			inline const std::string& GetName() const { return m_name; }
			inline const int& GetSortRank() const { return m_sort_rank; }
			const util::math::vec2 ConvertPointFromScreenToWorld(util::math::vec2 m_screen_point);
			const util::math::vec2 ConvertPointFromWorldToScreen(util::math::vec2 m_world_point);
		};
	}
}