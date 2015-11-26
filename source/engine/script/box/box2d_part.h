#pragma once
#include "script/renderable/render_part.h"
#include "Box2D/box2d.h"

namespace engine
{
	namespace script
	{
		class CBox2DPart : public script::renderable::IRenderPart
		{
		private:
			static b2World* s_pWorld;

		public:
			static void SetupWorld();
			static void DestroyWorld();
			static void UpdateWorld(const util::Time& dt);

			__todo() //will this be screwed up when we scale???
			static const float PIX_TO_BOX;
			static const float BOX_TO_PIX;


			/*Data*/
		private:
		protected:
			bool m_bSettingMatrix;

			/*Func*/
		public:
			CBox2DPart();
			virtual ~CBox2DPart();
			CBox2DPart& operator= (const CBox2DPart& that);

			virtual void Init();
			virtual void LoadJson(const util::JSON& json);

			virtual void OnMatrixChanged(void);
		};
	}
}