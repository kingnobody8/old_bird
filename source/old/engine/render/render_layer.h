#pragma once
#include "engine/asdf.h"
#include "engine/platform/inc_sdl.h"

namespace Engine
{
	namespace Render
	{
		//Forward Declare
		class IRenderNode;
		class CCamera;

		class CRenderLayer
		{
		private:
			std::list<IRenderNode*> m_vNodes;
			std::list<IRenderNode*> m_vCulledNodes;
			CCamera* m_pCamera;

		private:
			void Cull(void);

		public:
			CRenderLayer(void);
			CRenderLayer(CCamera* pCam);
			void DrawLayer(SDL_Renderer* pRen);
			void RegisterNode(IRenderNode* pNode);
			void UnregisterNode(IRenderNode* pNode);
			void ClearAllNodes(void);
			inline CCamera* const GetCamera(void) const { return m_pCamera; }
		};
	}
}