#pragma once
#include "Box2D/Box2D.h"

namespace engine
{
	namespace physics
	{
		struct GLRenderPoints;
		struct GLRenderLines;
		struct GLRenderTriangles;

		struct Camera
		{
			Camera()
			{
				m_center.Set(0.0f, 20.0f);
				m_extent = 25.0f;
				m_zoom = 1.0f;
//#ifdef WIN
//				m_width = 1280;
//				m_height = 720;
//#elif IOS
				m_width = 640;
				m_height = 1136;
//#endif
			}

			b2Vec2 ConvertScreenToWorld(const b2Vec2& screenPoint);
			b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPoint);
			void BuildProjectionMatrix(float32* m, float32 zBias);

			b2Vec2 m_center;
			float32 m_extent;
			float32 m_zoom;
			int32 m_width;
			int32 m_height;
		};

		// This class implements debug drawing callbacks that are invoked inside b2World::Step.
		class DebugDraw : public b2Draw
		{
		public:
			DebugDraw();
			~DebugDraw();

			void Create();
			void Destroy();

			void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

			void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

			void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

			void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

			void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

			void DrawTransform(const b2Transform& xf);

			void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

			void DrawAABB(b2AABB* aabb, const b2Color& color);

			void DrawParticles(const b2Vec2 *centers, const b2Vec2 *velocities, float32 radius, const b2ParticleColor *colors, int32 count);

			void Flush();

		private:
			GLRenderPoints* m_points;
			GLRenderLines* m_lines;
			GLRenderTriangles* m_triangles;
		};

		extern Camera g_camera;
	}
}


