#include "matrix_stack.h"
#include <assert.h>

namespace util
{
	namespace math
	{
		void MatrixStack::Push(const Matrix2D& mat)
		{
			this->m_cMat *= mat;
			this->stack.push(mat);
		}
		void MatrixStack::Pop(void)
		{
			assert(!this->stack.empty());
			Matrix2D top = this->stack.top();
			this->stack.pop();
			this->m_cMat *= Matrix2D::Matrix_Inverse(top);
		}
	}
}