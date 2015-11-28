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
				, m_bodyType(b2BodyType::b2_dynamicBody)
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
				util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();

				vec2 pos = wmat.GetPosition();
				util::shape::AABB aabb = m_pOwner->CalcAabb();
				const vec2 extends = aabb.CalcExtends();

				const b2Vec2 b2points[4] =
				{
					b2Vec2(-extends.x * PIX_TO_BOX, -extends.y * PIX_TO_BOX),
					b2Vec2(extends.x * PIX_TO_BOX, -extends.y * PIX_TO_BOX),
					b2Vec2(extends.x * PIX_TO_BOX, extends.y * PIX_TO_BOX),
					b2Vec2(-extends.x * PIX_TO_BOX, extends.y * PIX_TO_BOX)
				};

				b2PolygonShape shape;
				shape.Set(b2points, 4);
				//shape.SetAsBox(float32(aabb.CalcExtends().x * PIX_TO_BOX), float32(aabb.CalcExtends().y * PIX_TO_BOX));

				b2BodyDef bd;
				bd.type = m_bodyType;
				bd.fixedRotation;
				bd.position = b2Vec2(pos.x * PIX_TO_BOX, pos.y * PIX_TO_BOX);
				bd.angle = wmat.GetRotationZ() * DEG_TO_RAD;
				m_pBody = s_pWorld->CreateBody(&bd);
				m_pFixture = m_pBody->CreateFixture(&shape, 1.0f);

				if (m_bodyType == b2BodyType::b2_dynamicBody)
					EnableUpdate();
			}

			VIRTUAL void CFixturePart::Exit()
			{
				s_pWorld->DestroyBody(m_pBody);
				m_pBody = nullptr;
				m_pFixture = nullptr;
			}

			VIRTUAL void CFixturePart::LoadJson(const util::JSON& json)
			{
				IBox2DPart::LoadJson(json);

				m_bodyType = (b2BodyType)(json["body_type"].GetInt());
			}

			VIRTUAL void CFixturePart::Update(const util::Time& dt)
			{
				b2Vec2 bpos = m_pBody->GetPosition();
				float rot = m_pBody->GetAngle() * RAD_TO_DEG;

				m_bSettingMatrix = true;
				m_pOwner->SetWorldPosXY(bpos.x * BOX_TO_PIX, bpos.y * BOX_TO_PIX);
				m_pOwner->SetWorldRotationZ(rot);
				m_bSettingMatrix = false;
			}

			VIRTUAL void CFixturePart::OnMatrixChanged()
			{
				if (m_bSettingMatrix) //we only want to respond to a matrix changed event, if someone else changes our matrix
					return;

				__todo()//we may want a soft assert here, because we probably should never be setting a box parts matrix outside of the initial setup

					util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				vec2 pos = wmat.GetPosition() * PIX_TO_BOX;
				m_pBody->SetTransform(b2Vec2(pos.x, pos.y), wmat.GetRotationZ() * DEG_TO_RAD);
				m_pBody->SetLinearVelocity(b2Vec2(0, 0));
				m_pBody->SetAngularVelocity(0);
				m_pBody->SetAwake(true);
			}
		}
	}
}