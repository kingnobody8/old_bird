#include "matrix_2d.h"
#include <stack>

namespace util
{
	namespace math
	{
		class MatrixStack
		{
		private:
			Matrix2D	m_cMat;
			std::stack<Matrix2D> stack;

		public:
			void Push(const Matrix2D& mat);
			void Pop(void);
			inline Matrix2D Mat(void) const { return this->m_cMat; }
			inline int Size(void) const { return (int)stack.size(); }
		};
	}
}