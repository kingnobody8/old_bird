#pragma once
#include "render_node.h"
#include <vector>

namespace engine
{
	namespace render
	{
		class LineNode : public IRenderNode
		{
		private: __todo() //maybe we should template this class so that we can have different types of verts
			VertexColor* m_vVerts;
			int	m_vertCount;
			int* m_vIndicies;
			int m_indexCount;

		protected:
			virtual void CalcAabbInternal();

		public:
			LineNode();
			virtual ~LineNode();
			virtual void operator () (const matrix& inv_cam);

			const std::vector<VertexColor> GetVerts();
			const std::vector<int> GetIndicies();

			void InitVBO(const std::vector<VertexColor>& verts);
			virtual void FreeVBO();
		};
	}
}