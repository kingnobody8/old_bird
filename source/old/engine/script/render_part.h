#pragma once
#include "engine/component/part.h"
#include "engine/math/rect.h"

namespace Engine
{
	//forward declare
	namespace Render{ class IRenderNode; }

	namespace Script
	{
		class IRenderPart : public Part::IPart
		{
		public:

			/*Data*/
		protected:
			std::string m_szLayer;

			/*Func*/
		protected:
		public:
			IRenderPart(void);
			virtual	~IRenderPart(void) = 0;
			virtual void LoadJson(const Json::JSON& json);
			//TODO make a save json func

			virtual Render::IRenderNode* GetRenderNode(void) = 0;
			virtual Engine::Math::AABB GetAABB(void) const = 0;
			std::string GetLayer(void) const { return this->m_szLayer; }

			virtual inline const bool IsRenderable(void) const { return true; }
		};
	}
}