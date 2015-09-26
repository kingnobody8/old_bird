#pragma once
#include "engine/asdf.h"

//forward declare
class b2World;

namespace Engine
{
	namespace Box
	{
		static const float PIX_TO_BOX = 0.015625f;
		static const float BOX_TO_PIX = 64.0f;


		class CBox
		{
			/*Data*/
		private:
			static CBox* instance;
			b2World*		m_pWorld;

			/*Func*/
		private:
			CBox(void);
			~CBox(void);

		public:
			static CBox* Get(void);
			static void DeleteInstance(void);

			void Update(Util::Time dt);

			inline b2World* GetWorld(void) const { return this->m_pWorld; }
		};
	}
}