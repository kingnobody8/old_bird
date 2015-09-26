#pragma once
#include "engine/component/part.h"
#include "engine/util/util.h"
#include "engine/math/math.h"
#include "engine/input/input_event.h"

namespace Engine
{
	namespace Ui
	{
		class CScale : public Part::IPart
		{
		public:
			DECLARE_PART_TYPE_INFO(CScale);
			/*Data*/
		protected:
			int m_eLatLock;
			int m_eVertLock;
			int m_eAspectLock;
			Math::Type2<int> m_tDefaultRes;
			Math::vec2 m_tOriginalPos;
			Math::vec2 m_tOriginalScale;

			/*Func*/
		protected:
			void HandleResolutionChange(const Type2<int>& old_res, const Type2<int>& new_res, const vec2& scale);
			PROTO_EVENT_CALLBACK(OnResolutionChange, Input::CResolutionChange);
		public:
			CScale(void);
			virtual	~CScale(void);
			virtual void LoadJson(const Json::JSON& json);
			//TODO make a save json func

			virtual void Init(void);
			virtual void Exit(void);
		};
	}
}