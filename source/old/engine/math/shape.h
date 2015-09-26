#pragma once

namespace Engine
{
	namespace Math
	{
		/*
		TODO:
		make a mathy things like points, lines, rays, segments, circles, rects, ect
		make intersection functions for each of them to each other
		*/

		enum EShape { BASE, POINT, LINE, CIRCLE, RECT, EAABB, TRIANGLE, NUM_TYPES };

		class IShape
		{
		protected:
			EShape m_eType;
		public:
			IShape(void) { this->m_eType = BASE; }
			virtual ~IShape(void) = 0;

			//Gets
			EShape GetType(void) const { return this->m_eType; }
		};
		inline IShape::~IShape(void) {/*LEAVE EMPTY - ABC*/}
		
	}
}