#pragma once
#include "engine/util/util.h"
using namespace Engine::Util;
#include "engine/util/color.h"
#include "engine/script/render_part.h"
#include "engine/render/render_node.h"

//forward declare
class b2Body;

#define BOX_DEBUG_DRAW 0

namespace Engine
{
	namespace Box
	{
		class CBoxPart :
#if BOX_DEBUG_DRAW
		public Script::IRenderPart
#else
		public Part::IPart
#endif
		{
			enum EBoxType
			{
				EBT_STATIC,
				EBT_DYNAMIC
			};

		public:
			DECLARE_PART_TYPE_INFO(CBoxPart);

			/*Data*/
		private:
		protected:
		public:
			b2Body* m_pBody;
			bool we_changed;
			EBoxType type;

			vec2 c1;
			vec2 c2;

#if BOX_DEBUG_DRAW
			Render::CRenderNodeLine	m_cLineNode;
#endif

			/*Func*/
		public:
			CBoxPart(void);
			virtual ~CBoxPart(void);
			CBoxPart& operator= (const CBoxPart& that);

			virtual void Copy(Part::IPart* const that);
			virtual void LoadJson(const Json::JSON& json);
			//TODO add a savejson func

			virtual void Init(void);
			virtual void Exit(void);
			virtual void Update(const Util::Time& dt);

			virtual void OnMatrixChanged(void);

			//Gets
			//Sets

#if BOX_DEBUG_DRAW
			virtual Render::IRenderNode* GetRenderNode(void);
			virtual Engine::Math::AABB GetAABB(void) const;
			std::string GetLayer(void) const { return this->m_szLayer; }
#endif
		};
	}
}
