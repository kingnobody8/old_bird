#include "engine/ui/scale.h"
#include "engine/component/object.h"

namespace Engine
{
	namespace Ui
	{
		DEFINE_PART_TYPE_INFO(CScale);

		//const int CScale::Type = ++IPart::s_nextPartTypeId;
		//const char* CScale::GetTypeName() const { return "CScale"; }
		//Part::IPart::RegisterPart("CScale", CScale::Type, Util::CNewType<CScale>());

		CScale::CScale(void)
			: m_eLatLock(-1)
			, m_eVertLock(-1)
			, m_eAspectLock(-1)
		{
			this->m_priority = Part::EPriorities::IMG;
			//this->m_szType = "CScale";
		}

		/*virtual*/	CScale::~CScale(void)
		{
		}

		/*virtual*/ void CScale::LoadJson(const Json::JSON& json)
		{
			IPart::LoadJson(json);

			std::string latLock = json["lat_lock"].GetString();
			std::string vertLock = json["vert_lock"].GetString();
			std::string aspectLock = json["aspect_lock"].GetString();

			//TODO might want to put in error checking, or maybe some locks could potentially be invalid for a reason

			if ("left" == latLock)
				m_eLatLock = ELateral::L_LEFT;
			else if ("mid" == latLock)
				m_eLatLock = ELateral::L_MID;
			else if ("right" == latLock)
				m_eLatLock = ELateral::L_RIGHT;

			if ("top" == vertLock)
				m_eVertLock = EVertical::V_TOP;
			else if ("mid" == vertLock)
				m_eVertLock = EVertical::V_MID;
			else if ("bot" == vertLock)
				m_eVertLock = EVertical::V_BOT;

			if ("x" == aspectLock)
				m_eAspectLock = EAxis::AXIS_X;
			else if ("y" == aspectLock)
				m_eAspectLock = EAxis::AXIS_Y;
		}

		/*virtual*/ void CScale::Init(void)
		{
			const Math::CMatrix2D& mat = this->m_pOwner->GetLocalMatrix();
			this->m_tOriginalPos = mat.GetPosition();
			this->m_tOriginalScale = mat.GetScale();
			//Input::CResolutionChange().Subscribe(this, BIND1(this, &CScale::OnResolutionChange)); //TODO we need to fix the whole scaling thing. it fights with box2d right now

			Input::CRequestResolution request;
			request.Broadcast();
			this->m_tDefaultRes = request.default_res;
			this->HandleResolutionChange(request.default_res, request.curr_res, request.scale);
		}
		
		/*virtual*/ void CScale::Exit(void)
		{
			Event::CDispatcher::Get()->UnsubscribeAll(this);
		}

		void CScale::HandleResolutionChange(const Type2<int>& old_res, const Type2<int>& new_res, const vec2& tScale)
		{
			float scale;
			switch (this->m_eAspectLock)
			{
			case EAxis::AXIS_X:
				scale = tScale.w;
				break;
			case EAxis::AXIS_Y:
				scale = tScale.h;
				break;
			}

			Math::CMatrix2D mat = this->m_pOwner->GetLocalMatrix();
			vec2 pos = this->m_tOriginalPos;
			vec2 half_new_res = vec2(new_res.w * 0.5f, new_res.h * 0.5f);
			vec2 half_default_res = vec2(this->m_tDefaultRes.w * 0.5f, this->m_tDefaultRes.h * 0.5f);

			switch (this->m_eLatLock)
			{
			case ELateral::L_LEFT:
				pos.x = -half_new_res.w + (m_tOriginalPos.x + half_default_res.w) * scale; // same thing just cleaned up (-half_new_res.w + (m_tOriginalPos.x - -half_default_res.w) * scale)
				break;
			case ELateral::L_MID:
				pos.x = this->m_tOriginalPos.x * scale;
				break;
			case ELateral::L_RIGHT:
				pos.x = half_new_res.w - (half_default_res.w - m_tOriginalPos.x) * scale;
				break;
			}

			switch (this->m_eVertLock)
			{
			case EVertical::V_TOP:
				pos.y = half_new_res.h - (half_default_res.h - m_tOriginalPos.y) * scale;
				break;
			case EVertical::V_MID:
				pos.y = this->m_tOriginalPos.y * scale;
				break;
			case EVertical::V_BOT:
				pos.y = -half_new_res.h + (m_tOriginalPos.y + half_default_res.h) * scale; // same thing just cleaned up (-half_new_res.h + (m_tOriginalPos.y - -half_default_res.h) * scale)
				break;
			}

			mat.SetPosition(pos);
			mat.SetScale(this->m_tOriginalScale * scale);
			this->m_pOwner->SetLocalMatrix(mat);
		}

		EVENT_CALLBACK(CScale, OnResolutionChange, Input::CResolutionChange)
		{
			this->HandleResolutionChange(event.old_res, event.new_res, event.scale);
			return false;
		}
	}
}
