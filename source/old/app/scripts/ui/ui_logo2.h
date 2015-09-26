#pragma once
#include "engine/component/part.h"
#include "engine/math/interp.h"
#include "engine/component/object.h"
#include "engine/script/img.h"
#include "engine/event/event.h"
#include "engine/util/util.h"
#include "engine/input/input_event.h"

using namespace Engine;
using namespace Util;

namespace App
{
	namespace Script
	{
		namespace Ui
		{
			class CUiLogo2 : public Engine::Part::IPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CUiLogo2);

				/*Data*/
			private:
				Engine::Math::CInterp m_cGroupMoveYInterp;
				Engine::Math::CInterp m_cGroupMoveXInterp;
				Engine::Math::CInterp m_cBackRotInterp;
				Engine::Math::CInterp m_cForeRotInterp;

				//Engine::Math::CInterp m_cEdgeRotateInterp;
				//Engine::Math::CInterp m_cOtherRotateInterp;
				//Engine::Math::CInterp m_cEdgeDMoveYInterp;
				//Engine::Math::CInterp m_cBackAlphaInterp;
				//Engine::Math::CInterp m_cBackMoveInterp;

				vec2 original_back_pos;


				//TODO may want to make 2 blue squares one big and one little to have their alpha's fade in
				// although we have to figure out what to do about where they overlap

				Engine::Object::CObject* back;
				Engine::Object::CObject* fore;
				//Engine::Object::CObject* edgeL;
				//Engine::Object::CObject* edgeD;
				//Engine::Object::CGroup* edge_group;

				//Engine::Script::CImg* backImg;
				//Engine::Script::CImg* foreImg;
				//Engine::Script::CImg* edgeLImg;
				//Engine::Script::CImg* edgeDImg;

				/*Func*/
			private:
				//void OnFinishGroupMoveY(void);
				//void OnFinishGroupMoveX(void);

				PROTO_EVENT_CALLBACK(OnRKey, Engine::Input::CMouseButtonUpEvent);

			public:
				CUiLogo2(void);
				virtual void Init(void);
				virtual void Exit(void);
				virtual void Update(const Util::Time& dt);
			};
		}
	}
}