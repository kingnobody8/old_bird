#pragma once
#include "component/part.h"
#include "render/render_layer.h"
#include "input/input.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{

			class CUiPart : public component::IPart
			{
			public:
				typedef std::list<CUiPart*> UiPartList;
				typedef std::list<CUiPart*>::iterator UiPartIter;

				//Internal
			private:
				static UiPartList s_touchParts;
				static UiPartList s_pendingTouchParts;
				static void CleanTouchParts(void);
				void DisableTouchImmediate(void);


				//External
			public:
				static void OnMouseButtonDown(const input::mouse_events::ButtonAction& action);
				static void OnMouseButtonUp(const input::mouse_events::ButtonAction& action);
				static void OnMouseMotion(const input::mouse_events::MotionAction& action);

			private:
				bool				m_bTouchEnabled;
				bool				m_bPendingTouch;
				u16					m_usTouchIndex;

				//------------------------------------------------------------------------------

			public:
				DECLARE_PART_TYPE_INFO(CUiPart);

				/*Data*/
			private:
			protected:
				util::shape::AABB m_aabb;
				float m_zed;
				render::CRenderLayer* m_pLayer;

				/*Func*/
			protected:
				void CalculateIntersectionRect();
				virtual bool OnMouseButtonDownInternal(const input::mouse_events::ButtonAction& action) { return true };
				virtual bool OnMouseButtonUpInternal(const input::mouse_events::ButtonAction& action) { return true };
				virtual bool OnMouseMotionInternal(const input::mouse_events::MotionAction& action) { return true };

			public:
				CUiPart();
				virtual ~CUiPart();
				CUiPart& operator= (const CUiPart& that);

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
}