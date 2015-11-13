#include "points_part.h"
#include "../script.h"
#include "render/render_node.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CPointsPart);

			CPointsPart::CPointsPart()
				: m_render_mode(INVALID)
			{
				m_priority = EPriorities::POINTS_PART;
			}
			VIRTUAL CPointsPart::~CPointsPart()
			{
				delete m_pNode;
			}
			CPointsPart& CPointsPart::operator = (const CPointsPart& that)
			{
				IRenderPart::operator=(that);
				return *this;
			}

			VIRTUAL void CPointsPart::Init()
			{
				IRenderPart::Init();
				m_force = true;
				SetPoints(m_points);
				SetRenderMode(m_render_mode);
				m_force = false;
			}


			VIRTUAL void CPointsPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				
				assert(json.HasMember("render_mode"));
				assert(json.HasMember("points"));

				m_render_mode = (ERenderMode)json["render_mode"].GetInt();
				m_points = ((const util::JSON&)(json["points"])).GetArray<util::math::vec2>();

				assert(m_render_mode > INVALID && m_render_mode < COUNT);
			}


			void CPointsPart::SetRenderMode(ERenderMode mode)
			{
				if (!m_force && (m_render_mode == mode))
					return;

				//stupidity handling
				if (m_points.size() == 1)
					mode = ERenderMode::POINTS;
				else if (m_points.size() == 2 && mode == ERenderMode::CLOSED_LINE)
					mode = ERenderMode::OPEN_LINE;

				m_render_mode = mode;
				SetupNodes();
			}
			void CPointsPart::SetPoints(const std::vector<util::math::vec2>& points)
			{
				assert(!points.empty());

				m_points = points;

				m_force = true;
				SetRenderMode(m_render_mode);
				m_force = false;

			}
			
			void CPointsPart::SetupNodes()
			{
				if (!m_nodes.empty())
				{
					for (int i = 0; i < (int)m_nodes.size(); ++i)
					{
						SafeDelete(m_nodes[i]);
					}
					m_nodes.clear();
				}

				switch (m_render_mode)
				{
				case ERenderMode::POINTS:
					SetupPoints();
					break;
				case ERenderMode::OPEN_LINE:
					SetupLines(m_points.size()-1);
					break;
				case ERenderMode::CLOSED_LINE:
					SetupLines(m_points.size());
					break;
				default:
					assert(false);
					break;
				}
			}


			VIRTUAL void CPointsPart::OnZedChanged(void)
			{

			}

			VIRTUAL void CPointsPart::OnMatrixChanged(void)
			{
				m_force = true;
				SetupNodes();
				m_force = false;
			}

			void CPointsPart::SetupPoints()
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				m_nodes.reserve(wpoints.size());
				for (int i = 0; i < wpoints.size(); ++i)
				{
					m_nodes.push_back(new render::CRenderNodePoint());
					m_nodes[i]->Register(m_szLayer);
					m_nodes[i]->SetColor(m_color.SDL());
					static_cast<render::CRenderNodePoint*>(m_nodes[i])->SetPoint(wpoints[i]);
				}
			}

			void CPointsPart::SetupLines(const int& count)
			{
				const std::vector<util::math::vec2> wpoints = CalcWorldPoints();

				m_nodes.reserve(count / 2);

				for (int i = 0; i < count; ++i)
				{
					const int pindex = (i + 1) % wpoints.size();
					const util::math::vec2& p1 = wpoints[i];
					const util::math::vec2& p2 = wpoints[pindex];

					m_nodes.push_back(new render::CRenderNodeLine());
					m_nodes[i]->Register(m_szLayer);
					m_nodes[i]->SetColor(m_color.SDL());
					static_cast<render::CRenderNodeLine*>(m_nodes[i])->SetLine(util::shape::Segment(p1, p2));
				}
			}

			const std::vector<util::math::vec2> CPointsPart::CalcWorldPoints() const
			{
				std::vector<util::math::vec2> ret;
				ret.reserve(m_points.size());
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				for (int i = 0; i < m_points.size(); ++i)
					ret.push_back(util::math::Matrix2D::Vector_Matrix_Multiply(m_points[i], wmat));
				return ret;
			}
		}
	}
}