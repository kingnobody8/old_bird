#pragma once
#include "engine/platform/inc_sdl.h"
#include "engine/math/matrix_2d.h"
#include <map>
#include "engine/render/camera.h"
#include "engine/render/render_layer.h"

namespace Engine
{
	//forward declare
	namespace Object { class CObject; class CGroup; }

	struct SDL_GROUP
	{
		SDL_GROUP()
			: ren(null)
			, win(null)
		{
		}
		SDL_GROUP(SDL_Renderer* pRen, SDL_Window* pWin)
			: ren(pRen)
			, win(pWin)
		{
		}
		SDL_Renderer* ren;
		SDL_Window* win;
	};

	namespace Render
	{
		//forward declare
		class IRenderNode;

		class CRenderer
		{
		private:
			static CRenderer* instance;
			CRenderer(void);
			~CRenderer(void);
		public:
			static CRenderer* Get(void);
			static void DeleteInstance(void);

			/*data*/
		private:
			SDL_Renderer* m_pRen;
			SDL_Window* m_pWin;
			std::map<std::string, CCamera*> m_vCameras;
			std::map<std::string, CRenderLayer*> m_vLayers;

		private:
			void SetupSDL(void);
			IRenderNode* FindRenderNode(Object::CObject* const pObj);
			void NullRootRender(void) const;

			/*func*/
		public:
			void Init(void);
			void Exit(void);
			void Update(const Util::Time& dt);
			void Render(Object::CGroup* const pRoot);

			CRenderLayer* GetLayer(const std::string& layer);
			CCamera* GetCamera(const std::string& camera);
			const std::map<std::string, CCamera*>& GetCameraList(void) const { return m_vCameras; }
			const std::map<std::string, CRenderLayer*>& GetLayerList(void) const { return m_vLayers; }
		
			//Gets
			inline SDL_Renderer* GetRen(void) const { return this->m_pRen; }
			inline SDL_Window* GetWin(void) const { return this->m_pWin; }
		};
	}
}