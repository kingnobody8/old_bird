#include "engine/box/box_part.h"
#include "engine/box/box.h"
#include <box2d/Box2D.h>
#include <box2d/Collision/Shapes/b2PolygonShape.h>
#include "engine/component/object.h"

namespace Engine
{
	namespace Box
	{
		DEFINE_PART_TYPE_INFO(CBoxPart);

		CBoxPart::CBoxPart(void)
			: m_pBody(null)
			, we_changed(false)
		{
			//this->m_szType = "CBoxPart";
			this->m_priority = Part::EPriorities::PHYSICS;
		}
		/*virtual*/ CBoxPart::~CBoxPart(void)
		{
		}
		CBoxPart& CBoxPart::operator= (const CBoxPart& that)
		{
			IPart::operator=(that);
			return *this;
		}

		/*virtual*/ void CBoxPart::Copy(Part::IPart* const that)
		{
			auto cast = dynamic_cast<CBoxPart* const>(that);
			assert(cast && "COPY MAY ONLY BE PERFORMED ON PARTS OF THE SAME TYPE");
			*this = *cast;
		}

		/*virtual*/ void CBoxPart::LoadJson(const Json::JSON& json)
		{
			IPart::LoadJson(json);

			std::string box_type = json["box_type"].GetString();
			if ("static" == box_type)
				type = EBT_STATIC;
			else if ("dynamic" == box_type)
				type = EBT_DYNAMIC;
		}

		/*virtual*/ void CBoxPart::Init(void)
		{
			b2World* pWorld = Box::CBox::Get()->GetWorld();


			if (type == EBT_DYNAMIC)
			{
				vec2 pos = m_pOwner->CalcWorldMatrix().GetPosition();
				Math::AABB aabb = m_pOwner->CalcAabb();

				float32 a = aabb.GetWidth() * 0.5f * PIX_TO_BOX;
				b2PolygonShape shape;
				shape.SetAsBox(a, a);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position = b2Vec2(pos.x, pos.y);
				b2Body* body = m_pBody = pWorld->CreateBody(&bd);
				body->CreateFixture(&shape, 1.0f);
			}
			else
			{
				b2BodyDef bd;
				b2Body* ground = m_pBody = pWorld->CreateBody(&bd);

				b2EdgeShape shape;
				vec2 pos = m_pOwner->CalcWorldMatrix().GetPosition();
				Math::AABB aabb = m_pOwner->CalcAabb();

				c1 = vec2(-aabb.GetWidth() * 0.5f, pos.y + aabb.GetHeight() * 0.5f);
				c2 = vec2(aabb.GetWidth() * 0.5f, pos.y - aabb.GetHeight() * 0.5f);

				shape.Set(b2Vec2(c1.x * PIX_TO_BOX, c1.y * PIX_TO_BOX), b2Vec2(c2.x * PIX_TO_BOX, c2.y * PIX_TO_BOX));
				ground->CreateFixture(&shape, 0.0f);
			}
			EnableUpdate();
		}
		/*virtual*/ void CBoxPart::Exit(void)
		{

		}

		/*virtual*/ void CBoxPart::Update(const Util::Time& dt)
		{
			if (m_pBody && type == EBT_DYNAMIC)
			{
				b2Vec2 bpos = m_pBody->GetPosition();
				float rot = m_pBody->GetAngle() * RAD_TO_DEG;
				CMatrix2D wmat = m_pOwner->CalcWorldMatrix();
				CMatrix2D lmat = m_pOwner->GetLocalMatrix();
				vec2 wpos = wmat.GetPosition();
				vec2 lpos = lmat.GetPosition();

				vec2 offset = vec2(bpos.x, bpos.y) * BOX_TO_PIX - wpos;

				lpos += offset;
				lmat.SetPosition(lpos);
				lmat.SetRotationZ(rot);
				we_changed = true;
				m_pOwner->SetLocalMatrix(lmat);
				we_changed = false;
			}
		}

		/*virtual*/ void CBoxPart::OnMatrixChanged(void)
		{
			if (m_pBody != null && !we_changed)
			{
				if (type == EBT_DYNAMIC)
				{
					CMatrix2D mat = this->m_pOwner->CalcWorldMatrix();
					vec2 pos = mat.GetPosition() * PIX_TO_BOX;
					m_pBody->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
					m_pBody->SetLinearVelocity(b2Vec2_zero);
					m_pBody->SetAngularVelocity(0.0f);
					m_pBody->SetAwake(true);
				}
				else
				{
					b2Fixture* fixA = m_pBody->GetFixtureList();
					m_pBody->DestroyFixture(fixA);

					b2EdgeShape shape;
					vec2 pos = m_pOwner->CalcWorldMatrix().GetPosition();
					Math::AABB aabb = m_pOwner->CalcAabb();

					shape.Set(b2Vec2(-aabb.GetWidth() * 0.5f * PIX_TO_BOX, (pos.y + aabb.GetHeight() * 0.5f) * PIX_TO_BOX), b2Vec2(aabb.GetWidth() * 0.5f * PIX_TO_BOX, (pos.y + aabb.GetHeight() * 0.5f) * PIX_TO_BOX));
					m_pBody->CreateFixture(&shape, 0.0f);
				}
			}
		}

#if BOX_DEBUG_DRAW
		/*virtual*/ Render::IRenderNode* CBoxPart::GetRenderNode(void)
		{
			vec2 pos = m_pOwner->CalcWorldMatrix().GetPosition();
			Math::AABB aabb = m_pOwner->CalcAabb();
			vec2 v1 = pos + aabb.GetExtends();
			vec2 v2 = pos - aabb.GetExtends();
			v2.y = v1.y;

			b2Fixture* pFix = m_pBody->GetFixtureList();
			b2AABB asdf = pFix->GetAABB(0);
			v1.x = pos.x + asdf.GetExtents().x * BOX_TO_PIX;
			v1.y = pos.y + asdf.GetExtents().y * BOX_TO_PIX;
			v2.x = pos.x - asdf.GetExtents().x * BOX_TO_PIX;
			v2.y = pos.y + asdf.GetExtents().y * BOX_TO_PIX;

			this->m_cLineNode.clr = CLR::WHITE.SDL();
			this->m_cLineNode.v1 = c1;
			this->m_cLineNode.v2 = c2;
			return &this->m_cLineNode;
		}

		/*virtual*/ Engine::Math::AABB CBoxPart::GetAABB(void) const
		{
			return Math::AABB(); //TODO ahhhH!
		}
#endif
	}
}