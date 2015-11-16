#pragma once
#include "component/part.h"
#include "render/render_layer.h"

namespace engine
{
	namespace script
	{
		class CTouchPart : public component::IPart
		{
		public:
			DECLARE_PART_TYPE_INFO(CTouchPart);

			/*Data*/
		private:
		protected:
			util::shape::AABB m_aabb;
			float m_zed;
			render::CRenderLayer* m_pLayer;

			/*Func*/
		protected:
			void CalculateIntersectionRect();

		public:
			CTouchPart();
			virtual ~CTouchPart();
			CTouchPart& operator= (const CTouchPart& that);

			virtual void Init();
			virtual void Exit();
			virtual void LoadJson(const util::JSON& json);

			virtual void OnMatrixChanged(void);
			virtual void OnChildMatricChanged(component::CObject* child);

			/*void SetLayer();
			void SetZedToTop;
			void SetZedToBottom;*/
		};
	}
}