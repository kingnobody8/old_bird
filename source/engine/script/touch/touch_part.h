#pragma once
#include "component/part.h"
#include "render/render_layer.h"
#include "input/input.h"

namespace engine
{
	namespace script
	{
		class CTouchPart : public component::IPart
		{
			//Internal
		private:
			static component::PartList s_touchParts;

			//External
		public:
			static void OnMouseButtonDown(const input::mouse_events::ButtonAction& action);
			static void OnMouseButtonUp(const input::mouse_events::ButtonAction& action);
			static void OnMouseMotion(const input::mouse_events::MotionAction& action);
			//------------------------------------------------------------------------------

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
			virtual bool OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action) = 0;
			virtual void OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action) = 0;
			virtual void OnMouseMotionInternal(const input::mouse_events::MotionAction& action) = 0;

		public:
			CTouchPart();
			virtual ~CTouchPart();
			CTouchPart& operator= (const CTouchPart& that);

			virtual void Init();
			virtual void Exit();
			virtual void LoadJson(const util::JSON& json);

			virtual void OnMatrixChanged(void);
			virtual void OnChildMatricChanged(component::CObject* child);

			void Register();
			void Unregister();

			/*void SetLayer();
			void SetZedToTop;
			void SetZedToBottom;*/
		};
	}
}