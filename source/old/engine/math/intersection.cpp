////#include "intersection.h"
////#include "../util/func.h"
////
////namespace Engine
////{
////	namespace Math
////	{
////		bool IntersectRectAndShape(const CRect& a, IShape* b)
////		{
////			switch (b->GetType())
////			{
////			case POINT:
////			case LINE:
////			case CIRCLE:
////				break;
////			case RECT:
////				return IntersectRect(a, *dynamic_cast<CRect*>(b));
////			case EAABB:
////			case TRIANGLE:
////			default:
////				break;
////			}
////			return false;
////		}
////
////		//Defined in header
////		bool IntersectShape(IShape* a, IShape* b)
//		{
//			switch (a->GetType())
//			{
//			case POINT:
//			case LINE:
//			case CIRCLE:
//			case RECT:
//				IntersectRectAndShape(*dynamic_cast<CRect*>(a), b);
//			case TRIANGLE:
//			default:
//				return false;
//			}
//			return false;
//		}
////
////		bool IntersectRect(const CRect& a, const CRect& b)
//		{
//			return IntersectAABB(ConvertRectToAabb(a), ConvertRectToAabb(b));
//		}
////		bool IntersectRect(const CRect& a, const CRect& b, CRect& out)
//		{
//			AABB aabb;
//			if (IntersectAABB(ConvertRectToAabb(a), ConvertRectToAabb(b), aabb))
//			{
//				out = ConvertAabbToRect(aabb);
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
////
////		bool IntersectAABB(const AABB& a, const AABB& b)
//		{
//			if (a.max.x <= b.min.x)
//				return false;
//			if (a.max.y <= b.min.y)
//				return false;
//			if (a.min.x >= b.min.x)
//				return false;
//			if (a.min.y >= b.min.y)
//				return false;
//			return true;
//		}
////		bool IntersectAABB(const AABB& a, const AABB& b, AABB& out)
//		{
//			if (IntersectAABB(a, b))
//			{
//				//Calculate the intersection shape
//				out.min.x = Max(a.min.x, b.min.x);
//				out.min.y = Max(a.min.y, b.min.y);
//				out.max.x = Min(a.max.x, b.max.x);
//				out.max.y = Min(a.max.y, b.max.y);
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
////
////	}
////}