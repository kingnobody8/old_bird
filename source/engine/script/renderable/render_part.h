#pragma once
#include "component/part.h"

namespace engine
{
	//forward declare
	namespace render { class IRenderNode; }

	namespace script
	{
		namespace renderable
		{
			class IRenderPart : public component::IPart
			{
			public:
				/*Data*/
			protected:
				std::string m_szLayer;
				util::Color m_color;
				render::IRenderNode* m_pNode;

				/*Func*/
			protected:
			public:
				IRenderPart(void);
				virtual ~IRenderPart(void) = 0;
				IRenderPart& operator= (const IRenderPart& that);
				virtual void LoadJson(const util::JSON& json);

				virtual void OnVisibilityChanged(const bool visible);

				virtual const util::shape::AABB CalcAABB(void);
				virtual inline const bool IsRenderable(void) const { return true; }

				inline const std::string& GetLayer(void) const { return this->m_szLayer; }
				inline render::IRenderNode* GetRenderNode(void) { return m_pNode; }
				const bool IsRegistered() const;
				inline const util::Color GetLocalColor() const { return m_color; }
				const util::Color GetWorldColor() const;

				void SetLocalColor(const util::Color& clr);
			};
		}
	}
}