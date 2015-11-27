#include "fixture_part.h"
#include "script/script.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			DEFINE_PART_TYPE_INFO(CFixturePart);

			CFixturePart::CFixturePart()
				: m_bSettingMatrix(false)
				, m_bodyType(BT_INVALID)
			{
				m_priority = script::FIXTURE_PART;
			}
			VIRTUAL CFixturePart::~CFixturePart()
			{
			}
			CFixturePart& CFixturePart::operator = (const CFixturePart& that)
			{
				return *this;
			}

			VIRTUAL void CFixturePart::Init()
			{
				if (m_bodyType == BT_DYNAMIC)
				{
					util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();

					vec2 pos = wmat.GetPosition();
					util::shape::AABB aabb = m_pOwner->CalcAabb();

					vec2 size = aabb.CalcSize();

					float32 x = size.x * 0.5f * PIX_TO_BOX;
					float32 y = size.y * 0.5f * PIX_TO_BOX;
					b2PolygonShape shape;
					shape.SetAsBox(x, y);

					b2BodyDef bd;
					bd.type = b2_dynamicBody;
					bd.position = b2Vec2(pos.x * PIX_TO_BOX, pos.y * PIX_TO_BOX);
					b2Body* body = m_pBody = s_pWorld->CreateBody(&bd);
					body->CreateFixture(&shape, 1.0f);
				}
				else
				{
					b2BodyDef bd;
					b2Body* ground = m_pBody = s_pWorld->CreateBody(&bd);

					//b2EdgeShape shape;
					vec2 pos = m_pOwner->CalcWorldMatrix().GetPosition();
					util::shape::AABB aabb = m_pOwner->CalcAabb();
					vec2 size = aabb.CalcSize();


					//c1 = vec2(-size.x, pos.y + size.y);
					//c2 = vec2(size.y, pos.y - size.y);

					//shape.Set(b2Vec2(c1.x * PIX_TO_BOX, c1.y * PIX_TO_BOX), b2Vec2(c2.x * PIX_TO_BOX, c2.y * PIX_TO_BOX));

					float32 x = size.x * 0.5f * PIX_TO_BOX;
					float32 y = size.y * 0.5f * PIX_TO_BOX;
					b2PolygonShape shape;
					shape.SetAsBox(x, y);
					ground->CreateFixture(&shape, 0.0f);
				}
				EnableUpdate();
			}

			VIRTUAL void CFixturePart::LoadJson(const util::JSON& json)
			{
				IBox2DPart::LoadJson(json);

				m_bodyType = (BODY_TYPE)(json["body_type"].GetInt());
			}
		
			VIRTUAL void CFixturePart::Update(const util::Time& dt)
			{
				if (m_pBody && m_bodyType == BT_DYNAMIC)
				{
					b2Vec2 bpos = m_pBody->GetPosition();
					float rot = m_pBody->GetAngle() * RAD_TO_DEG;

					m_bSettingMatrix = true;
					m_pOwner->SetWorldPosXY(bpos.x * BOX_TO_PIX, bpos.y * BOX_TO_PIX);
					m_pOwner->SetWorldRotationZ(rot);
					m_bSettingMatrix = false;


				/*	util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
					util::math::Matrix2D lmat = m_pOwner->GetLocalMatrix();
					vec2 wpos = wmat.GetPosition();
					vec2 lpos = lmat.GetPosition();

					vec2 offset = vec2(bpos.x, bpos.y) * BOX_TO_PIX - wpos;

					lpos += offset;
					lmat.SetPosition(lpos);
					lmat.SetRotationZ(rot);
					m_bSettingMatrix = true;
					m_pOwner->SetLocalMatrix(lmat);
					m_bSettingMatrix = false;*/
				}
			}

			VIRTUAL void CFixturePart::OnMatrixChanged()
			{
				if (m_bSettingMatrix) //we only want to respond to a matrix changed event, if someone else changes our matrix
					return;

				util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				vec2 pos = wmat.GetPosition() * PIX_TO_BOX;
				m_pBody->SetTransform(b2Vec2(pos.x, pos.y), wmat.GetRotationZ() * DEG_TO_RAD);
				m_pBody->SetLinearVelocity(b2Vec2(0,0));
				m_pBody->SetAngularVelocity(0); __todo() //should we maintain this?
				m_pBody->SetAwake(true);
			}
		}
	}
}