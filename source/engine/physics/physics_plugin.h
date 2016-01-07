#pragma once
#include "plugin.h"
#include "box2d/Box2D.h"

namespace engine
{
	namespace physics
	{
		class PhysicsPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(PhysicsPlugin);

		private:
			SDL_GLContext m_pGLContext;
			SDL_Window* m_pSdlWin;

		public:
			PhysicsPlugin();
			virtual ~PhysicsPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

		};
	}
}