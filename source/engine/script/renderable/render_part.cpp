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
			{
			}
			IRenderPart& IRenderPart::operator = (const IRenderPart& that)
			{
				m_szLayer = that.m_szLayer;
				m_color = that.m_color;
				__todo() //is this ok? dereferencing the node
				*m_pNode = *that.m_pNode;
				return *this;
			}
			VIRTUAL	IRenderPart::~IRenderPart(void)
			{
			}

			VIRTUAL void IRenderPart::Init()
			{
				SetLayer(m_szLayer);
				SetLocalColor(m_color);
				OnMatrixChanged();
				OnZedChanged();
			}

			VIRTUAL void IRenderPart::LoadJson(const util::JSON& json)
			{
				m_szLayer = json["layer"].GetString();
				m_color = ((const util::JSON&)(json["color"])).GetColor();
			}

			VIRTUAL void IRenderPart::OnVisibilityChanged(const bool visible)
			{
				if (visible)
					m_pNode->Register(m_szLayer);
				else
					m_pNode->Unregister();
			}

			VIRTUAL const util::shape::AABB IRenderPart::CalcAABB(void)
			{
				return m_pNode->CalcAABB();
			}

			const bool IRenderPart::IsRegistered() const
			{
				return (m_pNode->GetLayer() != null);
			}
			const util::Color IRenderPart::GetWorldColor() const
			{
				util::Color clr;
				clr.SDL(m_pNode->GetColor());
				return clr;
			}

			void IRenderPart::SetLocalColor(const util::Color& clr)
			{
				__todo() //this needs to do a vertical search for ColorModPart and see what colors it applies to us so we can properly set the nod
				m_color = clr;
				m_pNode->SetColor(clr.SDL());
			}
			void IRenderPart::SetLayer(const std::string& szLayer)
			{
				m_szLayer = szLayer;
				if (m_pOwner->GetWorldVisible())
					m_pNode->Register(szLayer);
			}
		}
	}
}