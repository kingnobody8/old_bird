
#include "debug_draw.h"
#include "../render/render_core.h"

namespace engine
{
	namespace physics
	{
#define BUFFER_OFFSET(i) ((GLvoid*)(i))

		Camera g_camera;

		b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2& ps)
		{
			float32 w = float32(m_width);
			float32 h = float32(m_height);
			float32 u = ps.x / w;
			float32 v = (h - ps.y) / h;

			float32 ratio = w / h;
			b2Vec2 extents(ratio * m_extent, m_extent);
			extents *= m_zoom;

			b2Vec2 lower = m_center - extents;
			b2Vec2 upper = m_center + extents;

			b2Vec2 pw;
			pw.x = (1.0f - u) * lower.x + u * upper.x;
			pw.y = (1.0f - v) * lower.y + v * upper.y;
			return pw;
		}

		b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2& pw)
		{
			float32 w = float32(m_width);
			float32 h = float32(m_height);
			float32 ratio = w / h;
			b2Vec2 extents(ratio * m_extent, m_extent);
			extents *= m_zoom;

			b2Vec2 lower = m_center - extents;
			b2Vec2 upper = m_center + extents;

			float32 u = (pw.x - lower.x) / (upper.x - lower.x);
			float32 v = (pw.y - lower.y) / (upper.y - lower.y);

			b2Vec2 ps;
			ps.x = u * w;
			ps.y = (1.0f - v) * h;
			return ps;
		}

		// Convert from world coordinates to normalized device coordinates.
		// http://www.songho.ca/opengl/gl_projectionmatrix.html
		void Camera::BuildProjectionMatrix(float32* m, float32 zBias)
		{
			float32 w = float32(m_width);
			float32 h = float32(m_height);
			float32 ratio = w / h;
			b2Vec2 extents(ratio * m_extent, m_extent);
			extents *= m_zoom;

			b2Vec2 lower = m_center - extents;
			b2Vec2 upper = m_center + extents;

			m[0] = 2.0f / (upper.x - lower.x);
			m[1] = 0.0f;
			m[2] = 0.0f;
			m[3] = 0.0f;

			m[4] = 0.0f;
			m[5] = 2.0f / (upper.y - lower.y);
			m[6] = 0.0f;
			m[7] = 0.0f;

			m[8] = 0.0f;
			m[9] = 0.0f;
			m[10] = 1.0f;
			m[11] = 0.0f;

			m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
			m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
			m[14] = zBias;
			m[15] = 1.0f;
		}

		//
		static void sCheckGLError()
		{
			GLenum errCode = glGetError();
			if (errCode != GL_NO_ERROR)
			{
				SDL_Log("OpenGL error = %d\n", errCode);
				assert(false);
			}
		}

		// Prints shader compilation errors
		static void sPrintLog(GLuint object)
		{
			GLint log_length = 0;
			if (glIsShader(object))
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
			else if (glIsProgram(object))
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
			else
			{
				SDL_Log("printlog: Not a shader or a program\n");
				return;
			}

			char* log = (char*)malloc(log_length);

			if (glIsShader(object))
				glGetShaderInfoLog(object, log_length, NULL, log);
			else if (glIsProgram(object))
				glGetProgramInfoLog(object, log_length, NULL, log);

			SDL_Log("%s", log);
			free(log);
		}


		//
		static GLuint sCreateShaderFromString(const char* source, GLenum type)
		{
			GLuint res = glCreateShader(type);
			const char* sources[] = { source };
			glShaderSource(res, 1, sources, NULL);
			glCompileShader(res);
			GLint compile_ok = GL_FALSE;

			//Shader log length
			int infoLogLength = 0;
			int maxLength = infoLogLength;

			//Get info string length
			glGetShaderiv(res, GL_INFO_LOG_LENGTH, &maxLength);

			//Allocate string
			char* infoLog = new char[maxLength];

			//Get info log
			glGetShaderInfoLog(res, maxLength, &infoLogLength, infoLog);
			if (infoLogLength > 0)
			{
				//Print Log
				SDL_Log("%s\n", infoLog);
			}

			return res;
		}

		// 
		static GLuint sCreateShaderProgram(const char* vs, const char* fs)
		{
			GLuint vsId = sCreateShaderFromString(vs, GL_VERTEX_SHADER);
			GLuint fsId = sCreateShaderFromString(fs, GL_FRAGMENT_SHADER);
			assert(vsId != 0 && fsId != 0);

			GLuint programId = glCreateProgram();
			glAttachShader(programId, vsId);
			glAttachShader(programId, fsId);
			glLinkProgram(programId);

			glDeleteShader(vsId);
			glDeleteShader(fsId);

			GLint status = GL_FALSE;
			glGetProgramiv(programId, GL_LINK_STATUS, &status);
			assert(status != GL_FALSE);

			return programId;
		}

		//
		struct GLRenderPoints
		{
			void Create()
			{
				const char* vs = \
					"uniform mat4 projectionMatrix;\n"
					"attribute vec2 v_position;\n"
					"attribute vec4 v_color;\n"
					"attribute float v_size;\n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	f_color = v_color;\n"
					"	gl_Position = projectionMatrix * vec4(v_position, 0.0, 1.0);\n"
					"   gl_PointSize = v_size;\n"
					"}\n";

				const char* fs = \
					"precision highp float; \n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	gl_FragColor = f_color;\n"
					"}\n";

				m_programId = sCreateShaderProgram(vs, fs);
				m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
				m_vertexAttribute = glGetAttribLocation(m_programId, "v_position");
				m_colorAttribute = glGetAttribLocation(m_programId, "v_color");
				m_sizeAttribute = glGetAttribLocation(m_programId, "v_size");

				// Generate
				//glGenBuffers(1, &m_vaoId);
				glGenBuffers(3, m_vboIds);

			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);
				glEnableVertexAttribArray(m_vertexAttribute);
				glEnableVertexAttribArray(m_colorAttribute);
				glEnableVertexAttribArray(m_sizeAttribute);

				// Vertex buffer
				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
				glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				glBufferData(GL_ARRAY_BUFFER, sizeof(m_sizes), m_sizes, GL_DYNAMIC_DRAW);

				sCheckGLError();

				// Cleanup
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_count = 0;
			}

			void Destroy()
			{
				//if (m_vaoId)
				//{
					//glDeleteBuffers(1, &m_vaoId);
					glDeleteBuffers(2, m_vboIds);
				//	m_vaoId = 0;
				//}

				if (m_programId)
				{
					glDeleteProgram(m_programId);
					m_programId = 0;
				}
			}

			void Vertex(const b2Vec2& v, const b2Color& c, float32 size)
			{
				if (m_count == e_maxVertices)
					Flush();

				m_vertices[m_count] = v;
				m_colors[m_count] = c;
				m_sizes[m_count] = size;
				++m_count;
			}

			void Flush()
			{
				if (m_count == 0)
					return;

				glUseProgram(m_programId);

				float32 proj[16] = { 0.0f };
				g_camera.BuildProjectionMatrix(proj, 0.0f);

				glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, proj);

				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float32), m_sizes);

				//glEnable(GL_PROGRAM_POINT_SIZE);
				glDrawArrays(GL_POINTS, 0, m_count);
				//glDisable(GL_PROGRAM_POINT_SIZE);

				sCheckGLError();

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glUseProgram(0);

				m_count = 0;
			}

			enum { e_maxVertices = 512 };
			b2Vec2 m_vertices[e_maxVertices];
			b2Color m_colors[e_maxVertices];
			float32 m_sizes[e_maxVertices];

			int32 m_count;

			//GLuint m_vaoId;
			GLuint m_vboIds[3];
			GLuint m_programId;
			GLint m_projectionUniform;
			GLint m_vertexAttribute;
			GLint m_colorAttribute;
			GLint m_sizeAttribute;
		};

		//
		struct GLRenderLines
		{
			void Create()
			{
				const char* vs = \
					"uniform mat4 projectionMatrix;\n"
					"attribute vec2 v_position;\n"
					"attribute vec4 v_color;\n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	f_color = v_color;\n"
					"	gl_Position = projectionMatrix * vec4(v_position, 0.0, 1.0);\n"
					"}\n";

				const char* fs = \
					"precision highp float; \n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	gl_FragColor = f_color;\n"
					"}\n";

				m_programId = sCreateShaderProgram(vs, fs);
				sCheckGLError();
				m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
				sCheckGLError();
				m_vertexAttribute = glGetAttribLocation(m_programId, "v_position");
				m_colorAttribute = glGetAttribLocation(m_programId, "v_color");

				// Generate
				//glGenBuffers(1, &m_vaoId);
				glGenBuffers(2, m_vboIds);
sCheckGLError();
				
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);
				glEnableVertexAttribArray(m_vertexAttribute);
				sCheckGLError();
				glEnableVertexAttribArray(m_colorAttribute);
sCheckGLError();
				
				// Vertex buffer
				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				sCheckGLError();
				glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				sCheckGLError();
				//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				sCheckGLError();
				glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				//glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

				sCheckGLError();

				// Cleanup
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_count = 0;
			}

			void Destroy()
			{
				//if (m_vaoId)
				//{
					//glDeleteBuffers(1, &m_vaoId);
					glDeleteBuffers(2, m_vboIds);
					//m_vaoId = 0;
				//}

				if (m_programId)
				{
					glDeleteProgram(m_programId);
					m_programId = 0;
				}
			}

			void Vertex(const b2Vec2& v, const b2Color& c)
			{
				if (m_count == e_maxVertices)
					Flush();

				m_vertices[m_count] = v;
				m_colors[m_count] = c;
				++m_count;
			}

			void Flush()
			{
				if (m_count == 0)
					return;

				GLint drawFboID = 0;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING, &drawFboID);
				
				glUseProgram(m_programId);

				float32 proj[16] = { 0.0f };
				g_camera.BuildProjectionMatrix(proj, 0.1f);
				
				glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, proj);
sCheckGLError();
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);

				glEnableVertexAttribArray(m_vertexAttribute);
				sCheckGLError();
				glEnableVertexAttribArray(m_colorAttribute);
sCheckGLError();
				// Vertex buffer
				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				//glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);
				glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(b2Vec2), m_vertices);
sCheckGLError();
				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				//glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);
				glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(b2Color), m_colors);
sCheckGLError();
				glDrawArrays(GL_LINES, 0, m_count);

				sCheckGLError();

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				sCheckGLError();
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glUseProgram(0);
				sCheckGLError();

				m_count = 0;
			}

			enum { e_maxVertices = 2 * 512 };
			b2Vec2 m_vertices[e_maxVertices];
			b2Color m_colors[e_maxVertices];

			int32 m_count;

			//GLuint m_vaoId;
			GLuint m_vboIds[2];
			GLuint m_programId;
			GLint m_projectionUniform;
			GLint m_vertexAttribute;
			GLint m_colorAttribute;
		};

			void GLRenderTriangles::Create()
			{
				useCamera = true;

				const char* vs = \
					"uniform mat4 projectionMatrix;\n"
					"attribute vec2 v_position;\n"
					"attribute vec4 v_color;\n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	f_color = v_color;\n"
					"	gl_Position = projectionMatrix * vec4(v_position, 0.0, 1.0);\n"
					"}\n";

				const char* fs = \
					"precision highp float; \n"
					"varying vec4 f_color;\n"
					"void main(void)\n"
					"{\n"
					"	gl_FragColor = f_color;\n"
					"}\n";

				m_programId = sCreateShaderProgram(vs, fs);
				m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
				m_vertexAttribute = glGetAttribLocation(m_programId, "v_position");
				m_colorAttribute = glGetAttribLocation(m_programId, "v_color");

				// Generate
				//glGenBuffers(1, &m_vaoId);
				glGenBuffers(2, m_vboIds);

				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);
				glEnableVertexAttribArray(m_vertexAttribute);
				glEnableVertexAttribArray(m_colorAttribute);

				// Vertex buffer
				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
				//glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

				sCheckGLError();

				// Cleanup
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_count = 0;
			}

			void GLRenderTriangles::Destroy()
			{
				//if (m_vaoId)
				//{
				//	glDeleteBuffers(1, &m_vaoId);
					glDeleteBuffers(2, m_vboIds);
				//	m_vaoId = 0;
				//}

				if (m_programId)
				{
					glDeleteProgram(m_programId);
					m_programId = 0;
				}
			}

			void GLRenderTriangles::Vertex(const b2Vec2& v, const b2Color& c)
			{
				if (m_count == e_maxVertices)
					Flush();

				m_vertices[m_count] = v;
				m_colors[m_count] = c;
				++m_count;
			}

			void GLRenderTriangles::Flush()
			{
				if (m_count == 0)
					return;

				glUseProgram(m_programId);

				float32 proj[16] = { 0.0f };
				g_camera.BuildProjectionMatrix(proj, 0.2f);

				if (!useCamera)
				{
					for (int x = 0; x < 4; ++x)
					{
						for (int y = 0; y < 4; ++y)
						{
							if (x == y)
							{
								proj[y * 4 + x] = 1;
							}
							else
							{
								proj[y * 4 + x] = 0;
							}
						}
					}
				}

				glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, proj);

				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vaoId);

				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
				//glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);
				glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(b2Vec2), m_vertices);

				//glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
				//glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);
				glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(b2Color), m_colors);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDrawArrays(GL_TRIANGLES, 0, m_count);
				glDisable(GL_BLEND);

				sCheckGLError();

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glUseProgram(0);

				m_count = 0;
			}

			

		//
		DebugDraw::DebugDraw()
		{
			m_points = NULL;
			m_lines = NULL;
			m_triangles = NULL;
		}

		//
		DebugDraw::~DebugDraw()
		{
			b2Assert(m_points == NULL);
			b2Assert(m_lines == NULL);
			b2Assert(m_triangles == NULL);
		}

		//
		void DebugDraw::Create()
		{
			m_points = new GLRenderPoints;
			m_points->Create();
			m_lines = new GLRenderLines;
			m_lines->Create();
			m_triangles = new GLRenderTriangles;
			m_triangles->Create();
		}

		//
		void DebugDraw::Destroy()
		{
			m_points->Destroy();
			delete m_points;
			m_points = NULL;

			m_lines->Destroy();
			delete m_lines;
			m_lines = NULL;

			m_triangles->Destroy();
			delete m_triangles;
			m_triangles = NULL;
		}

		//
		void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
		{
			b2Vec2 p1 = vertices[vertexCount - 1];
			for (int32 i = 0; i < vertexCount; ++i)
			{
				b2Vec2 p2 = vertices[i];
				m_lines->Vertex(p1, color);
				m_lines->Vertex(p2, color);
				p1 = p2;
			}
		}

		//
		void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
		{
			b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

			for (int32 i = 1; i < vertexCount - 1; ++i)
			{
				m_triangles->Vertex(vertices[0], fillColor);
				m_triangles->Vertex(vertices[i], fillColor);
				m_triangles->Vertex(vertices[i + 1], fillColor);
			}

			b2Vec2 p1 = vertices[vertexCount - 1];
			for (int32 i = 0; i < vertexCount; ++i)
			{
				b2Vec2 p2 = vertices[i];
				m_lines->Vertex(p1, color);
				m_lines->Vertex(p2, color);
				p1 = p2;
			}
		}

		//
		void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
		{
			const float32 k_segments = 16.0f;
			const float32 k_increment = 2.0f * b2_pi / k_segments;
			float32 sinInc = sinf(k_increment);
			float32 cosInc = cosf(k_increment);
			b2Vec2 r1(1.0f, 0.0f);
			b2Vec2 v1 = center + radius * r1;
			for (int32 i = 0; i < k_segments; ++i)
			{
				// Perform rotation to avoid additional trigonometry.
				b2Vec2 r2;
				r2.x = cosInc * r1.x - sinInc * r1.y;
				r2.y = sinInc * r1.x + cosInc * r1.y;
				b2Vec2 v2 = center + radius * r2;
				m_lines->Vertex(v1, color);
				m_lines->Vertex(v2, color);
				r1 = r2;
				v1 = v2;
			}
		}

		//
		void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
		{
			const float32 k_segments = 16.0f;
			const float32 k_increment = 2.0f * b2_pi / k_segments;
			float32 sinInc = sinf(k_increment);
			float32 cosInc = cosf(k_increment);
			b2Vec2 v0 = center;
			b2Vec2 r1(cosInc, sinInc);
			b2Vec2 v1 = center + radius * r1;
			b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
			for (int32 i = 0; i < k_segments; ++i)
			{
				// Perform rotation to avoid additional trigonometry.
				b2Vec2 r2;
				r2.x = cosInc * r1.x - sinInc * r1.y;
				r2.y = sinInc * r1.x + cosInc * r1.y;
				b2Vec2 v2 = center + radius * r2;
				m_triangles->Vertex(v0, fillColor);
				m_triangles->Vertex(v1, fillColor);
				m_triangles->Vertex(v2, fillColor);
				r1 = r2;
				v1 = v2;
			}

			r1.Set(1.0f, 0.0f);
			v1 = center + radius * r1;
			for (int32 i = 0; i < k_segments; ++i)
			{
				b2Vec2 r2;
				r2.x = cosInc * r1.x - sinInc * r1.y;
				r2.y = sinInc * r1.x + cosInc * r1.y;
				b2Vec2 v2 = center + radius * r2;
				m_lines->Vertex(v1, color);
				m_lines->Vertex(v2, color);
				r1 = r2;
				v1 = v2;
			}

			// Draw a line fixed in the circle to animate rotation.
			b2Vec2 p = center + radius * axis;
			m_lines->Vertex(center, color);
			m_lines->Vertex(p, color);
		}

		//
		void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
		{
			m_lines->Vertex(p1, color);
			m_lines->Vertex(p2, color);
		}

		float smoothstep(float x) { return x * x * (3 - 2 * x); }
		float currentscale = 1;
		//
		void DebugDraw::DrawParticles(const b2Vec2 *centers, const b2Vec2 *velocities, float32 radius, const b2ParticleColor *colors, int32 count)
		{
			for (int i = 0; i < count; ++i)
			{
				b2Vec2 center = centers[i];
				b2Color color = b2Color(colors[i].r / 255.0f, colors[i].g / 255.0f, colors[i].b / 255.0f);

				/*b2Vec2 vel = velocities[i];
				vel.Normalize();

				m_lines->Vertex(center - vel, color);
				m_lines->Vertex(center, color);*/

				const float32 k_segments = 16.0f;
				const float32 k_increment = 2.0f * b2_pi / k_segments;
				float32 sinInc = sinf(k_increment);
				float32 cosInc = cosf(k_increment);
				b2Vec2 v0 = center;
				b2Vec2 r1(cosInc, sinInc);
				b2Vec2 v1 = center + radius * r1;
				float32 len = velocities[i].Length();
				float32 mod = len;
				b2Color fillColor(0.5f * color.r * mod, 0.5f * color.g * mod, 0.5f * color.b * mod, 0.5f);
				for (int32 i = 0; i < k_segments; ++i)
				{
					// Perform rotation to avoid additional trigonometry.
					b2Vec2 r2;
					r2.x = cosInc * r1.x - sinInc * r1.y;
					r2.y = sinInc * r1.x + cosInc * r1.y;
					b2Vec2 v2 = center + radius * r2;
					m_triangles->Vertex(v0, fillColor);
					m_triangles->Vertex(v1, fillColor);
					m_triangles->Vertex(v2, fillColor);
					r1 = r2;
					v1 = v2;
				}
			}
		}


		//
		void DebugDraw::DrawTransform(const b2Transform& xf)
		{
			const float32 k_axisScale = 0.4f;
			b2Color red(1.0f, 0.0f, 0.0f);
			b2Color green(0.0f, 1.0f, 0.0f);
			b2Vec2 p1 = xf.p, p2;

			m_lines->Vertex(p1, red);
			p2 = p1 + k_axisScale * xf.q.GetXAxis();
			m_lines->Vertex(p2, red);

			m_lines->Vertex(p1, green);
			p2 = p1 + k_axisScale * xf.q.GetYAxis();
			m_lines->Vertex(p2, green);
		}

		void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
		{
			m_points->Vertex(p, color, size);
		}

		void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
		{
			b2Vec2 p1 = aabb->lowerBound;
			b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
			b2Vec2 p3 = aabb->upperBound;
			b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

			m_lines->Vertex(p1, c);
			m_lines->Vertex(p2, c);

			m_lines->Vertex(p2, c);
			m_lines->Vertex(p3, c);

			m_lines->Vertex(p3, c);
			m_lines->Vertex(p4, c);

			m_lines->Vertex(p4, c);
			m_lines->Vertex(p1, c);
		}

		//
		void DebugDraw::Flush()
		{
			m_triangles->Flush();
			m_lines->Flush();
			m_points->Flush();
		}
	}
}