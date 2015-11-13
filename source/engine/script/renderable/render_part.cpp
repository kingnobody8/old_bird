#include "render_part.h"
#include "render/render_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			IRenderPart::IRenderPart(void)
				: m_pNode(null)
				, m_force(false)
			{
			}
			IRenderPart& IRenderPart::operator = (const IRenderPart& that)
			{
				m_szLayer = that.m_szLayer;
				m_color = that.m_color;
				__todo() //is this ok? dereferencing the node
				*m_pNode = *that.m_pNode;
				m_force = that.m_force;
				return *this;
			}
			VIRTUAL	IRenderPart::~IRenderPart(void)
			{
			}

			VIRTUAL void IRenderPart::Init()
			{
				m_force = true;
				SetLayer(m_szLayer);
				SetLocalColor(m_color);
				OnMatrixChanged();
				OnZedChanged();
				m_force = false;
			}

			VIRTUAL void IRenderPart::LoadJson(const util::JSON& json)
			{
				assert(json.HasMember("layer"));
				assert(json.HasMember("color"));

				m_szLayer = json["layer"].GetString();
				m_color = ((const util::JSON&)(json["color"])).GetColor();
			}

			VIRTUAL void IRenderPart::OnZedChanged(void)
			{
				if (m_pNode == null)
					return;
				const float wzed = m_pOwner->CalcWorldZed();
				m_pNode->SetZed(wzed);
			}

			VIRTUAL void IRenderPart::OnVisibilityChanged(const bool visible)
			{
				if (m_pNode == null)
					return;
				if (!m_force && (visible == (m_pNode->GetLayer() != null)))
					return;

				if (visible)
					m_pNode->Register(m_szLayer);
				else
					m_pNode->Unregister();
			}

			VIRTUAL const util::shape::AABB IRenderPart::CalcAABB(void)
			{
				if (m_pNode == null)
					return util::shape::AABB::INVALID_AABB;

				return m_pNode->CalcAABB();
			}

			const bool IRenderPart::IsRegistered() const
			{
				if (m_pNode == null)
					return false;

				return (m_pNode->GetLayer() != null);
			}
			const util::Color IRenderPart::GetWorldColor() const
			{
				if (m_pNode == null)
					return util::Color::WHITE;

				util::Color clr;
				clr.SDL(m_pNode->GetColor());
				return clr;
			}

			void IRenderPart::SetLocalColor(const util::Color& clr)
			{
				if (!m_force && (m_color == clr))
					return;

				if (m_pNode == null)
					return;

				__todo() //this needs to do a vertical search for ColorModPart and see what colors it applies to us so we can properly set the nod
				m_color = clr;
				m_pNode->SetColor(clr.SDL());
			}
			void IRenderPart::SetLayer(const std::string& szLayer)
			{
				if (m_pNode == null)
					return;
				if (!m_force && (m_szLayer == szLayer))
					return;

				m_szLayer = szLayer;
				if (m_pOwner->GetWorldVisible())
					m_pNode->Register(szLayer);
			}

		
		}
	}
}