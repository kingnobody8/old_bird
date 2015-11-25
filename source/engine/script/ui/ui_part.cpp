#include "ui_part.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace ui
		{
			DEFINE_PART_TYPE_INFO(CUiPart);

			bool SortFunc(CUiPart* lhs, CUiPart* rhs)
			{
				return true;
				//if (lhs->GetLayer() == rhs->GetLayer())
				//	return lhs->GetZed() < rhs->GetZed();
				//return lhs->GetLayer()->GetSortRank() > rhs->GetLayer()->GetSortRank();
			}

			/*bool SortLayerFunc(const CUiPart::UiLayer& lhs, const CUiPart::UiLayer& rhs)
			{

			}*/

			static const u16 INVALID_TOUCH_INDEX = 0xFFFF;

			STATIC std::list<CUiPart::UiLayer> CUiPart::s_uiLayers;
			STATIC CUiPart::UiPartList CUiPart::s_pendingUiParts;

			STATIC const CUiPart::UiPartList CUiPart::GetUiParts()
			{
				UiPartList ret;
				for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
				{
					ret.insert(ret.end(), (*iter).m_uiParts.begin(), (*iter).m_uiParts.end());
				}
				return ret;
			}

			STATIC const CUiPart::UiPartList CUiPart::GetUiPartsOnLayer(const std::string& szLayer)
			{
				UiPartList ret;
				for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
				{
					if ((*iter).m_pLayer->GetName() == szLayer)
					{
						ret.insert(ret.end(), (*iter).m_uiParts.begin(), (*iter).m_uiParts.end());
						break;
					}
				}
				return ret;
			}


			STATIC void CUiPart::CleanTouchParts(void)
			{
				bool appended = false;
				for (UiPartIter iter = s_pendingUiParts.begin(); iter != s_pendingUiParts.end(); ++iter)
				{
					CUiPart* part = (*iter);
					part->m_bPendingTouch = false;


					UiPartList* list = null;
					for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
					{
						if ((*iter).m_pLayer->GetName() == part->m_szLayer)
						{
							list = &(*iter).m_uiParts;
							break;
						}
					}
					assert(list != null);


					if (part->m_bTouchEnabled) //Enable
					{
						// Guard against enabling multiple times
						if (part->m_usTouchIndex == INVALID_TOUCH_INDEX)
						{
							part->m_usTouchIndex = (u16)list->size();
							list->push_back(part);
							appended = true;
						}
					}
					else //Disable
					{
						if (part->m_usTouchIndex != INVALID_TOUCH_INDEX)
						{
							if (!list->empty())
							{
								UiPartIter ours = list->begin();
								std::advance(ours, part->m_usTouchIndex);
								UiPartIter back = list->end();
								std::advance(back, -1);
								std::swap(ours, back);
								list->pop_back();
							}
							//Invalidate the index
							part->m_usTouchIndex = INVALID_TOUCH_INDEX;
						}
					}
				}
				s_pendingUiParts.clear();

				if (appended)
				{
					__todo()
					//Sort the layers

					//Sort the parts in each layer
				}
			}

			STATIC void CUiPart::OnMouseButtonDown(const input::mouse_events::ButtonAction& action)
			{
				CleanTouchParts();

				for (std::list<CUiPart::UiLayer>::iterator layer_iter = s_uiLayers.begin(); layer_iter != s_uiLayers.end(); ++layer_iter)
				{
					const vec2 wpos = (*layer_iter).m_pLayer->ConvertPointFromScreenToWorld(vec2(action.m_pixel.x, action.m_pixel.y));

					for (UiPartIter part_iter = (*layer_iter).m_uiParts.begin(); part_iter != (*layer_iter).m_uiParts.end(); ++part_iter)
					{
						if ((*part_iter)->m_bTouchEnabled)
						{
							if (!(*part_iter)->OnMouseButtonDownInternal(action, wpos))
								return;
						}
					}
				}
			}
			
			STATIC void CUiPart::OnMouseButtonUp(const input::mouse_events::ButtonAction& action)
			{
				CleanTouchParts();
				
				for (std::list<CUiPart::UiLayer>::iterator layer_iter = s_uiLayers.begin(); layer_iter != s_uiLayers.end(); ++layer_iter)
				{
					const vec2 wpos = (*layer_iter).m_pLayer->ConvertPointFromScreenToWorld(vec2(action.m_pixel.x, action.m_pixel.y));

					for (UiPartIter part_iter = (*layer_iter).m_uiParts.begin(); part_iter != (*layer_iter).m_uiParts.end(); ++part_iter)
					{
						if ((*part_iter)->m_bTouchEnabled)
						{
							if (!(*part_iter)->OnMouseButtonUpInternal(action, wpos))
								return;
						}
					}
				}
			}

			STATIC void CUiPart::OnMouseMotion(const input::mouse_events::MotionAction& action)
			{
				CleanTouchParts();
				
				for (std::list<CUiPart::UiLayer>::iterator layer_iter = s_uiLayers.begin(); layer_iter != s_uiLayers.end(); ++layer_iter)
				{
					const vec2 wpos = (*layer_iter).m_pLayer->ConvertPointFromScreenToWorld(vec2(action.m_pixel.x, action.m_pixel.y));

					for (UiPartIter part_iter = (*layer_iter).m_uiParts.begin(); part_iter != (*layer_iter).m_uiParts.end(); ++part_iter)
					{
						if ((*part_iter)->m_bTouchEnabled)
						{
							if (!(*part_iter)->OnMouseMotionInternal(action, wpos))
								return;
						}
					}
				}
			}

			CUiPart::CUiPart()
				: m_bTouchEnabled(false)
				, m_bPendingTouch(false)
				, m_usTouchIndex(INVALID_TOUCH_INDEX)
			{
			}

			CUiPart::~CUiPart()
			{
				DisableTouchImmediate();
			}

			void CUiPart::CalculateIntersectionRect()
			{
				m_aabb = m_pOwner->CalcAabb();
			}

			VIRTUAL void CUiPart::Init()
			{
				Register();
				CalculateIntersectionRect();
			}

			VIRTUAL void CUiPart::Exit()
			{
			}

			VIRTUAL void CUiPart::LoadJson(const util::JSON& json)
			{
				m_szLayer = json["layer"].GetString();
				assert(!m_szLayer.empty());
			}

			VIRTUAL void CUiPart::OnMatrixChanged(void)
			{
				CalculateIntersectionRect();
			}

			VIRTUAL void CUiPart::OnChildMatrixChanged(component::CObject* child)
			{
				CalculateIntersectionRect();
			}

			void CUiPart::Register()
			{
				if (m_bTouchEnabled)
					return;
				m_bTouchEnabled = true;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingUiParts.push_back(this);

				bool found = false;
				for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
				{
					if ((*iter).m_pLayer->GetName() == this->m_szLayer)
					{
						found = true;
						break;
					}
				}

				__todo() //initialize these from the render layers themselves, instead of here
				if (!found)
				{
					CUiPart::UiLayer tmp;
					tmp.m_pLayer = render::CRenderLayer::FindLayer(m_szLayer);
					assert(tmp.m_pLayer != null);
					s_uiLayers.push_back(tmp);
					__todo() //sort the layers
				}

			}

			void CUiPart::Unregister()
			{
				if (!m_bTouchEnabled)
					return;
				m_bTouchEnabled = false;

				if (m_bPendingTouch)
					return;
				m_bPendingTouch = true;

				s_pendingUiParts.push_back(this);
			}

			void CUiPart::DisableTouchImmediate(void)
			{
				if (this->m_usTouchIndex != INVALID_TOUCH_INDEX)
				{
					UiPartList* list = null;
					for (auto iter = s_uiLayers.begin(); iter != s_uiLayers.end(); ++iter)
					{
						if ((*iter).m_pLayer->GetName() == this->m_szLayer)
						{
							list = &(*iter).m_uiParts;
							break;
						}
					}
					assert(list != null);


					if (!list->empty())
					{
						UiPartIter ours = list->begin();
						std::advance(ours, this->m_usTouchIndex);
						UiPartIter back = list->end();
						std::advance(back, -1);
						std::swap(ours, back);
						list->pop_back();
					}
					//Invalidate the index
					this->m_usTouchIndex = INVALID_TOUCH_INDEX;
				}
			}
		}
	}
}