#include "click_part.h"
#include "engine/component/object.h"
#include "engine/component/object.inl"
#include <Box2D/Collision/Shapes/b2Shape.h>
#include "engine/box/box_part.h"
#include <box2d/Box2D.h>
#include "engine/box/box.h"
#include "engine/render/renderer.h"

namespace Engine
{
	namespace Script
	{
		DEFINE_PART_TYPE_INFO(CClickPart);

		CClickPart::CClickPart(void)
			: m_mdown(false)
			, m_ldown(false)
			, m_rdown(false)
		{
			//this->m_szType = "CClickPart";
			this->m_priority = Part::EPriorities::INPUT;
			type = 0;
		}
		CClickPart::~CClickPart(void)
		{
		}

		/*virtual*/ void CClickPart::Init(void)
		{
			Input::CMouseButtonDownEvent().Subscribe(this, BIND1(this, &CClickPart::OnMouseDown));
			Input::CMouseMotionEvent().Subscribe(this, BIND1(this, &CClickPart::OnMouseMove));
			Input::CMouseButtonUpEvent().Subscribe(this, BIND1(this, &CClickPart::OnMouseUp));

			Input::CResolutionChange().Subscribe(this, BIND1(this, &CClickPart::OnResChanged));
			m_res.Broadcast();

			Engine::Input::CKeyUpEvent().Subscribe(this, BIND1(this, &CClickPart::OnKeyUp));
			EnableUpdate();

			Render::CRenderer::Get()->GetCamera("world")->SetFollow(m_pOwner);
		}

		/*virtual*/ void CClickPart::Exit(void)
		{
			Event::CDispatcher::Get()->UnsubscribeAll(this);
		}

		/*virtual*/ void CClickPart::Update(const Util::Time& dt)
		{
			if (m_rdown)
			{
				SetPosition(mouse_world_pos);
			}
			if (m_ldown)
			{
				switch (type)
				{
				case 1:
					DoPush(mouse_world_pos);
					break;
				case 2:
					DoPull(mouse_world_pos);
					break;
				}
			}
		}

		void CClickPart::SetPosition(vec2 pos)
		{
			m_pOwner->SetWorldPosXY(pos);

			//CMatrix2D wmat = m_pOwner->CalcWorldMatrix();
			//CMatrix2D lmat = m_pOwner->CalcWorldMatrix(); //TODO we need a function to set world position, it will set the local so that the world matches
			//vec2 wpos = wmat.GetPosition();
			//vec2 lpos = lmat.GetPosition();
			//
			//vec2 offset = pos - wpos;
			//
			//lpos += offset;
			//lmat.SetPosition(lpos);
			//m_pOwner->SetLocalMatrix(lmat);
		}

		EVENT_CALLBACK(CClickPart, OnResChanged, Engine::Input::CResolutionChange)
		{
			m_res.curr_res = event.new_res;
			m_res.scale = event.scale;
			return false;
		}
		EVENT_CALLBACK(CClickPart, OnMouseDown, Engine::Input::CMouseButtonDownEvent)
		{
			mouse_world_pos = vec2(event.pixel.x - m_res.curr_res.x / 2.0f, event.pixel.y - m_res.curr_res.y / 2.0f) - Render::CRenderer::Get()->GetCamera("world")->GetMatrix().GetPosition();
			mouse_world_pos = vec2(event.layer_pos["world"].x, event.layer_pos["world"].y);
			if (event.button == SDL_BUTTON_RIGHT)
			{
				SetPosition(mouse_world_pos);
				m_rdown = true;
			}
			else if (event.button == SDL_BUTTON_LEFT)
			{
				m_ldown = true;
			}
			return false;
		}
		EVENT_CALLBACK(CClickPart, OnMouseMove, Engine::Input::CMouseMotionEvent)
		{
			mouse_world_pos = vec2(event.pixel.x - m_res.curr_res.x / 2.0f, event.pixel.y - m_res.curr_res.y / 2.0f) - Render::CRenderer::Get()->GetCamera("world")->GetMatrix().GetPosition();
			mouse_world_pos = vec2(event.layer_pos["world"].x, event.layer_pos["world"].y);
			vec2 vel = event.velocity * 1000.0f;

			if (m_rdown)
			{
				SetPosition(mouse_world_pos);
			}
			else if (m_ldown)
			{
				switch (type)
				{
				case 0:
					DoSwipe(vel);
					break;
				}
			}
			return false;
		}
		EVENT_CALLBACK(CClickPart, OnMouseUp, Engine::Input::CMouseButtonUpEvent)
		{
			m_mdown = false;
			m_ldown = false;
			m_rdown = false;
			return false;
		}
		EVENT_CALLBACK(CClickPart, OnKeyUp, Engine::Input::CKeyUpEvent)
		{
			switch (event.code)
			{
			case SDLK_1: //swipe
				type = 0;
				break;
			case SDLK_2: //push
				type = 1;
				break;
			case SDLK_3: //pull
				type = 2;
				break;
			}
			return false;
		}

		void CClickPart::DoPush(vec2 pos)
		{
			CMatrix2D wmat = m_pOwner->CalcWorldMatrix();
			vec2 wpos = wmat.GetPosition();
			vec2 offset = wpos - pos;
			offset.normalize();
			offset *= 500.0f;

			Box::CBoxPart* pBox = m_pOwner->FindPart<Box::CBoxPart>();
			pBox->m_pBody->ApplyForceToCenter(b2Vec2(offset.x * Box::PIX_TO_BOX, offset.y * Box::PIX_TO_BOX), true);
		}
		void CClickPart::DoPull(vec2 pos)
		{
			CMatrix2D wmat = m_pOwner->CalcWorldMatrix();
			vec2 wpos = wmat.GetPosition();
			vec2 offset = pos - wpos;
			offset.normalize();
			offset *= 500.0f;

			Box::CBoxPart* pBox = m_pOwner->FindPart<Box::CBoxPart>();
			pBox->m_pBody->ApplyForceToCenter(b2Vec2(offset.x * Box::PIX_TO_BOX, offset.y * Box::PIX_TO_BOX), true);
		}
		void CClickPart::DoSwipe(vec2 vel)
		{
			Box::CBoxPart* pBox = m_pOwner->FindPart<Box::CBoxPart>();
			pBox->m_pBody->ApplyForceToCenter(b2Vec2(vel.x * Box::PIX_TO_BOX, vel.y * Box::PIX_TO_BOX), true);
		}
	}
}
