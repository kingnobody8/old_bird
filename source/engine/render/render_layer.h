#pragma once
#include "sdl/include/SDL.h"
#include <list>
#include "macro.h"
#include <string>
#include "math/math.h"

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
			void Cull(const b2AABB& view);

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
			const vec2 ConvertPointFromScreenToWorld(vec2 m_screen_point);
			const vec2 ConvertPointFromWorldToScreen(vec2 m_world_point);
		};
	}
}