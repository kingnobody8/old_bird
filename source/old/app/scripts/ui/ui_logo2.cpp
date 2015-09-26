#include "ui_logo2.h"
#include "engine/component/object.h"
#include "engine/script/img.h"
#include "engine/asset/asset.h"
#include "engine/event/dispatcher.h"
#include "engine/input/input_event.h"

namespace App
{
	namespace Script
	{
		namespace Ui
		{
			DEFINE_PART_TYPE_INFO(CUiLogo2);

			CUiLogo2::CUiLogo2(void)
				: back(null)
				, fore(null)
			{
				//this->m_szType = "CUiLogo";
				this->m_priority = Part::EPriorities::APP;
			}

			/*virtual*/ void CUiLogo2::Init(void)
			{
				//Initialize data
				Engine::Object::CGroup* pGroup = dynamic_cast<Engine::Object::CGroup*>(this->m_pOwner);
				this->back = pGroup->FindObject("core");
				this->fore = pGroup->FindObject("edge");

				//Set the initial matrix
				Input::CRequestResolution res;
				res.Broadcast();

				CMatrix2D mat = m_pOwner->GetLocalMatrix();
				mat.SetRotationZ(-135.0f);
				m_pOwner->SetLocalMatrix(mat);

				CMatrix2D bMat, fMat;
				bMat = this->back->CalcWorldMatrix();
				fMat = this->fore->CalcWorldMatrix();

				mat.SetRotationZ(0);
				m_pOwner->SetLocalMatrix(mat);

				//this->back->SetWorldPosXY(bMat.GetPosition());
				//this->back->SetWorldRotationZ(bMat.GetRotationZ());
				this->back->SetLocalRotationZ(bMat.GetRotationZ());
				
				//this->fore->SetWorldPosXY(fMat.GetPosition());
				//this->fore->SetWorldRotationZ(fMat.GetRotationZ());
				this->fore->SetLocalRotationZ(-135.0f);

				Engine::Math::Interpolator::Overshoot interp;
				interp.over_shoot_ratio = 0.15f;
				interp.pass_point_ratio = 0.60f;
				m_cBackRotInterp.Init(bMat.GetRotationZ(), 0, 1.5f, interp);
				m_cForeRotInterp.Init(-135.0f, -360.0f, 1.5f, interp);

				//mat = this->fore->GetLocalMatrix();
				//mat = Math::Matrix_Create_Rotation_Z(90.0f) * mat;
				//this->fore->SetLocalMatrix(mat);

				//float lower_y_pos = -res.curr_res.h * 0.5f -this->m_pOwner->CalcAabb().GetHeight() * 0.5f;
				//vec2 back_offset = vec2(1.0f, 1.0f) * (res.curr_res.w * 0.5f + backImg->GetAABB().GetWidth() * 0.5f);

				//mat = this->back->GetLocalMatrix();
				//vec2 pos = original_back_pos = mat.GetPosition();
				//vec2 scale = this->back->CalcWorldMatrix().GetScale();
				//pos += back_offset / scale;
				//mat.SetPosition(pos);
				//this->back->SetLocalMatrix(mat);

				////Begin animation
				////this->backImg->SetAlpha(0.0f);
				//Engine::Math::Interpolator::Overshoot interp;
				//interp.over_shoot_ratio = 0.10f;
				//interp.pass_point_ratio = 0.70f;

				//this->m_cGroupMoveYInterp.Init(lower_y_pos, 0,
				//	1.0f, interp, 
				//	BIND0(this, &CUiLogo2::OnFinishGroupMoveY));
				////#num -40 make the logo 'look' centered
				//Engine::Math::Interpolator::Linear linear_interp;
				//m_cGroupMoveXInterp.Init(0, -40.0f,
				//	2.5f, linear_interp,
				//	BIND0(this, &CUiLogo2::OnFinishGroupMoveX));
				Engine::Input::CMouseButtonUpEvent().Subscribe(this, BIND1(this, &CUiLogo2::OnRKey));
				//Engine::Event::CDispatcher::Get()->CallbackDelay(BIND0(this,
				//	&CUiLogo2::OnInitEdgeRotate),
				//	0.75f);

				//TODO this is just for reset purposes
				/*mat = this->m_pOwner->GetLocalMatrix();
				pos = mat.GetPosition();
				pos.x = 0.0f;
				mat.SetPosition(pos);
				this->m_pOwner->SetLocalMatrix(mat);*/
			}
			/*virtual*/ void CUiLogo2::Exit(void)
			{
				m_cGroupMoveYInterp.Reset();
				m_cGroupMoveXInterp.Reset();

				m_cBackRotInterp.Reset();
				m_cForeRotInterp.Reset();

				Engine::Input::CMouseButtonUpEvent().Unsubscribe(this);
				Engine::Event::CDispatcher::Get()->UnsubscribeAll(this);
			}
			/*virtual*/ void CUiLogo2::Update(const Util::Time& dt)
			{
				CMatrix2D mat = this->m_pOwner->GetLocalMatrix();
				vec2 pos = mat.GetPosition();
				vec2 scale = mat.GetScale();
				float rot = mat.GetRotationZ();

				CMatrix2D bWMat, fWMat, bLMat, fLMat;
				bWMat = this->back->CalcWorldMatrix();
				fWMat = this->fore->CalcWorldMatrix();
				bLMat = this->back->GetLocalMatrix();
				fLMat = this->fore->GetLocalMatrix();

				//back->SetLocalMatrix(Math::Matrix_Create_Rotation_Z(15.0f * dt) * bLMat);
				fore->SetLocalMatrix(Math::Matrix_Create_Rotation_Z(-15.0f * dt) * fLMat);

				//this->back->SetWorldRotationZ(bMat.GetRotationZ());
				//this->fore->SetWorldRotationZ(fMat.GetRotationZ());

				if (m_cBackRotInterp.IsInterpolating())
				{
					m_cBackRotInterp.Update(dt);
					bLMat.SetRotationZ(m_cBackRotInterp.Poll());
				}

				back->SetLocalMatrix(bLMat);

				if (m_cForeRotInterp.IsInterpolating())
				{
					m_cForeRotInterp.Update(dt);
					fLMat.SetRotationZ(m_cForeRotInterp.Poll());
				}
				fore->SetLocalMatrix(fLMat);

				if (this->m_cGroupMoveYInterp.IsInterpolating())
				{
					this->m_cGroupMoveYInterp.Update(dt);
					pos.y = this->m_cGroupMoveYInterp.Poll();
				}
				if (this->m_cGroupMoveXInterp.IsInterpolating())
				{
					this->m_cGroupMoveXInterp.Update(dt);
					pos.x = this->m_cGroupMoveXInterp.Poll();
				}
				/*if (this->m_cEdgeRotateInterp.IsInterpolating())
				{
					CMatrix2D edge_mat = this->edge_group->GetLocalMatrix();
					this->m_cEdgeRotateInterp.Update(dt);
					edge_mat.SetRotationZ(this->m_cEdgeRotateInterp.Poll());
					this->edge_group->SetLocalMatrix(edge_mat);
				}
				if (this->m_cOtherRotateInterp.IsInterpolating())
				{
					CMatrix2D other_mat = this->fore->GetLocalMatrix();
					this->m_cOtherRotateInterp.Update(dt);
					other_mat.SetRotationZ(this->m_cOtherRotateInterp.Poll());
					this->fore->SetLocalMatrix(other_mat);
				}
				if (this->m_cBackAlphaInterp.IsInterpolating())
				{
					this->m_cBackAlphaInterp.Update(dt);
					this->backImg->SetAlpha(this->m_cBackAlphaInterp.Poll());
				}
				if (this->m_cBackMoveInterp.IsInterpolating())
				{
					this->m_cBackMoveInterp.Update(dt);
					CMatrix2D other_mat = this->back->GetLocalMatrix();
					vec2 pos = vec2(1.0f, 1.0f) * this->m_cBackMoveInterp.Poll();
					other_mat.SetPosition(pos);
					this->back->SetLocalMatrix(other_mat);
				}*/

				mat.SetPosition(pos);
				mat.SetScale(scale);
				mat.SetRotationZ(rot);

				this->m_pOwner->SetLocalMatrix(mat);
			}

	//		void CUiLogo2::OnFinishGroupMoveY()
	//		{
	//		
	//		}

	//		void CUiLogo2::OnFinishGroupMoveX()
	//		{
	//			//TODO add a interp to pop up the dot piece
	//			//m_cEdgeDMoveYInterp.Init(0, 400.0f, 0.5f, )

	//			//TODO make this end when the everything ends
	//			vec2 pos = this->back->GetLocalMatrix().GetPosition();
	//			this->m_cBackMoveInterp.Init(pos.x, original_back_pos.x,
	//				0.2f, Engine::Math::Interpolator::Linear());

	//		}

	//		void CUiLogo2::OnInitEdgeRotate(void)
	//		{
	//			Engine::Math::Interpolator::EaseInSine interp;
	//			//interp.expo = 2;
	//			m_cEdgeRotateInterp.Init(-135.0f, -720.0f, 1.75f, interp);

	//			Engine::Math::Interpolator::Overshoot overshoot;
	//			overshoot.pass_point_ratio = 0.65f;
	//			overshoot.over_shoot_ratio = 0.15f;
	//				m_cOtherRotateInterp.Init(-135.0f, -360.0f,
	//					1.75f, overshoot,
	//					BIND0(this, &CUiLogo2::OnFinishGroupRotate));
	//		}

	//		void CUiLogo2::OnFinishGroupRotate(void)
	//		{
	//			Engine::Math::Interpolator::EaseInExpo interp;
	//			interp.expo = 2;

	////			m_cBackAlphaInterp.Init(0, 1.0f,
	//	//			0.25f, interp);
	//			this->backImg->SetAlpha(1.0f);
	//		}

			EVENT_CALLBACK(CUiLogo2, OnRKey, Engine::Input::CMouseButtonUpEvent)
			{
				//if (event.code == SDLK_r && !event.repeat)
				//{
					this->Reset();
				//}
				return false;
			}
		}
	}
}