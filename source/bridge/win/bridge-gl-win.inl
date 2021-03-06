MACRO(PFNGLACTIVETEXTUREPROC, void, glActiveTexture, (GLenum texture), pfn(texture), return)
MACRO(PFNGLATTACHSHADERPROC, void, glAttachShader, (GLuint program, GLuint shader), pfn(program, shader), return)
MACRO(PFNGLBINDATTRIBLOCATIONPROC, void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar* name), pfn(program, index, name), return)
MACRO(PFNGLBINDBUFFERPROC, void, glBindBuffer, (GLenum target, GLuint buffer), pfn(target, buffer), return)
MACRO(PFNGLBINDFRAMEBUFFERPROC, void, glBindFramebuffer, (GLenum target, GLuint framebuffer), pfn(target, framebuffer), return)
MACRO(PFNGLBINDRENDERBUFFERPROC, void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer), pfn(target, renderbuffer), return)
//MACRO(PFNGLBINDTEXTUREPROC, void, glBindTexture, (GLenum target, GLuint texture), pfn(target, texture), return)
MACRO(PFNGLBLENDCOLORPROC, void, glBlendColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha), pfn(red, green, blue, alpha), return)
MACRO(PFNGLBLENDEQUATIONPROC, void, glBlendEquation, (GLenum mode), pfn(mode), return)
MACRO(PFNGLBLENDEQUATIONSEPARATEPROC, void, glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha), pfn(modeRGB, modeAlpha), return)
//MACRO(PFNGLBLENDFUNCPROC, void, glBlendFunc, (GLenum sfactor, GLenum dfactor), pfn(sfactor, dfactor), return)
MACRO(PFNGLBLENDFUNCSEPARATEPROC, void, glBlendFuncSeparate, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha), pfn(srcRGB, dstRGB, srcAlpha, dstAlpha), return)
MACRO(PFNGLBUFFERDATAPROC, void, glBufferData, (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage), pfn(target, size, data, usage), return)
MACRO(PFNGLBUFFERSUBDATAPROC, void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data), pfn(target, offset, size, data), return)
MACRO(PFNGLCHECKFRAMEBUFFERSTATUSPROC, GLenum, glCheckFramebufferStatus, (GLenum target), return pfn(target), return 0)
//MACRO(PFNGLCLEARPROC, void, glClear, (GLbitfield mask), pfn(mask), return)
//MACRO(PFNGLCLEARCOLORPROC, void, glClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha), pfn(red, green, blue, alpha), return)
MACRO(PFNGLCLEARDEPTHFPROC, void, glClearDepthf, (GLclampf depth), pfn(depth), return)
//MACRO(PFNGLCLEARSTENCILPROC, void, glClearStencil, (GLint s), pfn(s), return)
//MACRO(PFNGLCOLORMASKPROC, void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha), pfn(red, green, blue, alpha), return)
MACRO(PFNGLCOMPILESHADERPROC, void, glCompileShader, (GLuint shader), pfn(shader), return)
MACRO(PFNGLCOMPRESSEDTEXIMAGE2DPROC, void, glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data), pfn(target, level, internalformat, width, height, border, imageSize, data), return)
MACRO(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, void, glCompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data), pfn(target, level, xoffset, yoffset, width, height, format, imageSize, data), return)
//MACRO(PFNGLCOPYTEXIMAGE2DPROC, void, glCopyTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border), pfn(target, level, internalformat, x, y, width, height, border), return)
//MACRO(PFNGLCOPYTEXSUBIMAGE2DPROC, void, glCopyTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height), pfn(target, level, xoffset, yoffset, x, y, width, height), return)
MACRO(PFNGLCREATEPROGRAMPROC, GLuint, glCreateProgram, (void), return pfn(), return 0)
MACRO(PFNGLCREATESHADERPROC, GLuint, glCreateShader, (GLenum type), return pfn(type), return 0)
//MACRO(PFNGLCULLFACEPROC, void, glCullFace, (GLenum mode), pfn(mode), return)
MACRO(PFNGLDELETEBUFFERSPROC, void, glDeleteBuffers, (GLsizei n, const GLuint* buffers), pfn(n, buffers), return)
MACRO(PFNGLDELETEFRAMEBUFFERSPROC, void, glDeleteFramebuffers, (GLsizei n, const GLuint* framebuffers), pfn(n, framebuffers), return)
MACRO(PFNGLDELETEPROGRAMPROC, void, glDeleteProgram, (GLuint program), pfn(program), return)
MACRO(PFNGLDELETERENDERBUFFERSPROC, void, glDeleteRenderbuffers, (GLsizei n, const GLuint* renderbuffers), pfn(n, renderbuffers), return)
MACRO(PFNGLDELETESHADERPROC, void, glDeleteShader, (GLuint shader), pfn(shader), return)
//MACRO(PFNGLDELETETEXTURESPROC, void, glDeleteTextures, (GLsizei n, const GLuint* textures), pfn(n, textures), return)
//MACRO(PFNGLDEPTHFUNCPROC, void, glDepthFunc, (GLenum func), pfn(func), return)
//MACRO(PFNGLDEPTHMASKPROC, void, glDepthMask, (GLboolean flag), pfn(flag), return)
MACRO(PFNGLDEPTHRANGEFPROC, void, glDepthRangef, (GLclampf zNear, GLclampf zFar), pfn(zNear, zFar), return)
MACRO(PFNGLDETACHSHADERPROC, void, glDetachShader, (GLuint program, GLuint shader), pfn(program, shader), return)
//MACRO(PFNGLDISABLEPROC, void, glDisable, (GLenum cap), pfn(cap), return)
MACRO(PFNGLDISABLEVERTEXATTRIBARRAYPROC, void, glDisableVertexAttribArray, (GLuint index), pfn(index), return)
//MACRO(PFNGLDRAWARRAYSPROC, void, glDrawArrays, (GLenum mode, GLint first, GLsizei count), pfn(mode, first, count), return)
//MACRO(PFNGLDRAWELEMENTSPROC, void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices), pfn(mode, count, type, indices), return)
//MACRO(PFNGLENABLEPROC, void, glEnable, (GLenum cap), pfn(cap), return)
MACRO(PFNGLENABLEVERTEXATTRIBARRAYPROC, void, glEnableVertexAttribArray, (GLuint index), pfn(index), return)
//MACRO(PFNGLFINISHPROC, void, glFinish, (void), pfn(void), return)
//MACRO(PFNGLFLUSHPROC, void, glFlush, (void), pfn(void), return)
MACRO(PFNGLFRAMEBUFFERRENDERBUFFERPROC, void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer), pfn(target, attachment, renderbuffertarget, renderbuffer), return)
MACRO(PFNGLFRAMEBUFFERTEXTURE2DPROC, void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level), pfn(target, attachment, textarget, texture, level), return)
//MACRO(PFNGLFRONTFACEPROC, void, glFrontFace, (GLenum mode), pfn(mode), return)
MACRO(PFNGLGENBUFFERSPROC, void, glGenBuffers, (GLsizei n, GLuint* buffers), pfn(n, buffers), return)
MACRO(PFNGLGENERATEMIPMAPPROC, void, glGenerateMipmap, (GLenum target), pfn(target), return)
MACRO(PFNGLGENFRAMEBUFFERSPROC, void, glGenFramebuffers, (GLsizei n, GLuint* framebuffers), pfn(n, framebuffers), return)
MACRO(PFNGLGENRENDERBUFFERSPROC, void, glGenRenderbuffers, (GLsizei n, GLuint* renderbuffers), pfn(n, renderbuffers), return)
//MACRO(PFNGLGENTEXTURESPROC, void, glGenTextures, (GLsizei n, GLuint* textures), pfn(n, textures), return)
MACRO(PFNGLGETACTIVEATTRIBPROC, void, glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name), pfn(program, index, bufsize, length, size, type, name), return)
MACRO(PFNGLGETACTIVEUNIFORMPROC, void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name), pfn(program, index, bufsize, length, size, type, name), return)
MACRO(PFNGLGETATTACHEDSHADERSPROC, void, glGetAttachedShaders, (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders), pfn(program, maxcount, count, shaders), return)
MACRO(PFNGLGETATTRIBLOCATIONPROC, GLint, glGetAttribLocation, (GLuint program, const GLchar* name), return pfn(program, name), return -1)
//MACRO(PFNGLGETBOOLEANVPROC, void, glGetBooleanv, (GLenum pname, GLboolean* params), pfn(pname, params), return)
MACRO(PFNGLGETBUFFERPARAMETERIVPROC, void, glGetBufferParameteriv, (GLenum target, GLenum pname, GLint* params), pfn(target, pname, params), return)
//MACRO(PFNGLGETERRORPROC, GLenum, glGetError, (void), return pfn(void), return 0)
//MACRO(PFNGLGETFLOATVPROC, void, glGetFloatv, (GLenum pname, GLfloat* params), pfn(pname, params), return)
MACRO(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, void, glGetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint* params), pfn(target, attachment, pname, params), return)
//MACRO(PFNGLGETINTEGERVPROC, void, glGetIntegerv, (GLenum pname, GLint* params), pfn(pname, params), return)
MACRO(PFNGLGETPROGRAMIVPROC, void, glGetProgramiv, (GLuint program, GLenum pname, GLint* params), pfn(program, pname, params), return)
MACRO(PFNGLGETPROGRAMINFOLOGPROC, void, glGetProgramInfoLog, (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog), pfn(program, bufsize, length, infolog), return)
MACRO(PFNGLGETRENDERBUFFERPARAMETERIVPROC, void, glGetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint* params), pfn(target, pname, params), return)
MACRO(PFNGLGETSHADERIVPROC, void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* params), pfn(shader, pname, params), return)
MACRO(PFNGLGETSHADERINFOLOGPROC, void, glGetShaderInfoLog, (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog), pfn(shader, bufsize, length, infolog), return)
MACRO(PFNGLGETSHADERPRECISIONFORMATPROC, void, glGetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision), pfn(shadertype, precisiontype, range, precision), return)
MACRO(PFNGLGETSHADERSOURCEPROC, void, glGetShaderSource, (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source), pfn(shader, bufsize, length, source), return)
//MACRO(PFNGLGETTEXPARAMETERFVPROC, void, glGetTexParameterfv, (GLenum target, GLenum pname, GLfloat* params), pfn(target, pname, params), return)
//MACRO(PFNGLGETTEXPARAMETERIVPROC, void, glGetTexParameteriv, (GLenum target, GLenum pname, GLint* params), pfn(target, pname, params), return)
MACRO(PFNGLGETUNIFORMFVPROC, void, glGetUniformfv, (GLuint program, GLint location, GLfloat* params), pfn(program, location, params), return)
MACRO(PFNGLGETUNIFORMIVPROC, void, glGetUniformiv, (GLuint program, GLint location, GLint* params), pfn(program, location, params), return)
MACRO(PFNGLGETUNIFORMLOCATIONPROC, GLint, glGetUniformLocation, (GLuint program, const GLchar* name), return pfn(program, name), return -1)
MACRO(PFNGLGETVERTEXATTRIBFVPROC, void, glGetVertexAttribfv, (GLuint index, GLenum pname, GLfloat* params), pfn(index, pname, params), return)
MACRO(PFNGLGETVERTEXATTRIBIVPROC, void, glGetVertexAttribiv, (GLuint index, GLenum pname, GLint* params), pfn(index, pname, params), return)
MACRO(PFNGLGETVERTEXATTRIBPOINTERVPROC, void, glGetVertexAttribPointerv, (GLuint index, GLenum pname, GLvoid** pointer), pfn(index, pname, pointer), return)
//MACRO(PFNGLHINTPROC, void, glHint, (GLenum target, GLenum mode), pfn(target, mode), return)
MACRO(PFNGLISBUFFERPROC, GLboolean, glIsBuffer, (GLuint buffer), return pfn(buffer), return -1)
//MACRO(PFNGLISENABLEDPROC, GLboolean, glIsEnabled, (GLenum cap), return pfn(cap), return -1)
MACRO(PFNGLISFRAMEBUFFERPROC, GLboolean, glIsFramebuffer, (GLuint framebuffer), return pfn(framebuffer), return -1)
MACRO(PFNGLISPROGRAMPROC, GLboolean, glIsProgram, (GLuint program), return pfn(program), return -1)
MACRO(PFNGLISRENDERBUFFERPROC, GLboolean, glIsRenderbuffer, (GLuint renderbuffer), return pfn(renderbuffer), return -1)
MACRO(PFNGLISSHADERPROC, GLboolean, glIsShader, (GLuint shader), return pfn(shader), return -1)
//MACRO(PFNGLISTEXTUREPROC, GLboolean, glIsTexture, (GLuint texture), return pfn(texture), return -1)
//MACRO(PFNGLLINEWIDTHPROC, void, glLineWidth, (GLfloat width), pfn(width), return)
MACRO(PFNGLLINKPROGRAMPROC, void, glLinkProgram, (GLuint program), pfn(program), return)
//MACRO(PFNGLPIXELSTOREIPROC, void, glPixelStorei, (GLenum pname, GLint param), pfn(pname, param), return)
//MACRO(PFNGLPOLYGONOFFSETPROC, void, glPolygonOffset, (GLfloat factor, GLfloat units), pfn(factor, units), return)
//MACRO(PFNGLREADPIXELSPROC, void, glReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels), pfn(x, y, width, height, format, type, pixels), return)
MACRO(PFNGLRELEASESHADERCOMPILERPROC, void, glReleaseShaderCompiler, (void), pfn(), return)
MACRO(PFNGLRENDERBUFFERSTORAGEPROC, void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height), pfn(target, internalformat, width, height), return)
MACRO(PFNGLSAMPLECOVERAGEPROC, void, glSampleCoverage, (GLclampf value, GLboolean invert), pfn(value, invert), return)
//MACRO(PFNGLSCISSORPROC, void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height), pfn(x, y, width, height), return)
MACRO(PFNGLSHADERBINARYPROC, void, glShaderBinary, (GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length), pfn(n, shaders, binaryformat, binary, length), return)
MACRO(PFNGLSHADERSOURCEPROC, void, glShaderSource, (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length), pfn(shader, count, string, length), return)
//MACRO(PFNGLSTENCILFUNCPROC, void, glStencilFunc, (GLenum func, GLint ref, GLuint mask), pfn(func, ref, mask), return)
MACRO(PFNGLSTENCILFUNCSEPARATEPROC, void, glStencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask), pfn(face, func, ref, mask), return)
//MACRO(PFNGLSTENCILMASKPROC, void, glStencilMask, (GLuint mask), pfn(mask), return)
MACRO(PFNGLSTENCILMASKSEPARATEPROC, void, glStencilMaskSeparate, (GLenum face, GLuint mask), pfn(face, mask), return)
//MACRO(PFNGLSTENCILOPPROC, void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass), pfn(fail, zfail, zpass), return)
MACRO(PFNGLSTENCILOPSEPARATEPROC, void, glStencilOpSeparate, (GLenum face, GLenum fail, GLenum zfail, GLenum zpass), pfn(face, fail, zfail, zpass), return)
//MACRO(PFNGLTEXIMAGE2DPROC, void, glTexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels), pfn(target, level, internalformat, width, height, border, format, type, pixels), return)
//MACRO(PFNGLTEXPARAMETERFPROC, void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param), pfn(target, pname, param), return)
//MACRO(PFNGLTEXPARAMETERFVPROC, void, glTexParameterfv, (GLenum target, GLenum pname, const GLfloat* params), pfn(target, pname, params), return)
//MACRO(PFNGLTEXPARAMETERIPROC, void, glTexParameteri, (GLenum target, GLenum pname, GLint param), pfn(target, pname, param), return)
//MACRO(PFNGLTEXPARAMETERIVPROC, void, glTexParameteriv, (GLenum target, GLenum pname, const GLint* params), pfn(target, pname, params), return)
//MACRO(PFNGLTEXSUBIMAGE2DPROC, void, glTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels), pfn(target, level, xoffset, yoffset, width, height, format, type, pixels), return)
MACRO(PFNGLUNIFORM1FPROC, void, glUniform1f, (GLint location, GLfloat x), pfn(location, x), return)
MACRO(PFNGLUNIFORM1FVPROC, void, glUniform1fv, (GLint location, GLsizei count, const GLfloat* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM1IPROC, void, glUniform1i, (GLint location, GLint x), pfn(location, x), return)
MACRO(PFNGLUNIFORM1IVPROC, void, glUniform1iv, (GLint location, GLsizei count, const GLint* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM2FPROC, void, glUniform2f, (GLint location, GLfloat x, GLfloat y), pfn(location, x, y), return)
MACRO(PFNGLUNIFORM2FVPROC, void, glUniform2fv, (GLint location, GLsizei count, const GLfloat* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM2IPROC, void, glUniform2i, (GLint location, GLint x, GLint y), pfn(location, x, y), return)
MACRO(PFNGLUNIFORM2IVPROC, void, glUniform2iv, (GLint location, GLsizei count, const GLint* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM3FPROC, void, glUniform3f, (GLint location, GLfloat x, GLfloat y, GLfloat z), pfn(location, x, y, z), return)
MACRO(PFNGLUNIFORM3FVPROC, void, glUniform3fv, (GLint location, GLsizei count, const GLfloat* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM3IPROC, void, glUniform3i, (GLint location, GLint x, GLint y, GLint z), pfn(location, x, y, z), return)
MACRO(PFNGLUNIFORM3IVPROC, void, glUniform3iv, (GLint location, GLsizei count, const GLint* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM4FPROC, void, glUniform4f, (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w), pfn(location, x, y, z, w), return)
MACRO(PFNGLUNIFORM4FVPROC, void, glUniform4fv, (GLint location, GLsizei count, const GLfloat* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORM4IPROC, void, glUniform4i, (GLint location, GLint x, GLint y, GLint z, GLint w), pfn(location, x, y, z, w), return)
MACRO(PFNGLUNIFORM4IVPROC, void, glUniform4iv, (GLint location, GLsizei count, const GLint* v), pfn(location, count, v), return)
MACRO(PFNGLUNIFORMMATRIX2FVPROC, void, glUniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value), pfn(location, count, transpose, value), return)
MACRO(PFNGLUNIFORMMATRIX3FVPROC, void, glUniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value), pfn(location, count, transpose, value), return)
MACRO(PFNGLUNIFORMMATRIX4FVPROC, void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value), pfn(location, count, transpose, value), return)
MACRO(PFNGLUSEPROGRAMPROC, void, glUseProgram, (GLuint program), pfn(program), return)
MACRO(PFNGLVALIDATEPROGRAMPROC, void, glValidateProgram, (GLuint program), pfn(program), return)
MACRO(PFNGLVERTEXATTRIB1FPROC, void, glVertexAttrib1f, (GLuint indx, GLfloat x), pfn(indx, x), return)
MACRO(PFNGLVERTEXATTRIB1FVPROC, void, glVertexAttrib1fv, (GLuint indx, const GLfloat* values), pfn(indx, values), return)
MACRO(PFNGLVERTEXATTRIB2FPROC, void, glVertexAttrib2f, (GLuint indx, GLfloat x, GLfloat y), pfn(indx, x, y), return)
MACRO(PFNGLVERTEXATTRIB2FVPROC, void, glVertexAttrib2fv, (GLuint indx, const GLfloat* values), pfn(indx, values), return)
MACRO(PFNGLVERTEXATTRIB3FPROC, void, glVertexAttrib3f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z), pfn(indx, x, y, z), return)
MACRO(PFNGLVERTEXATTRIB3FVPROC, void, glVertexAttrib3fv, (GLuint indx, const GLfloat* values), pfn(indx, values), return)
MACRO(PFNGLVERTEXATTRIB4FPROC, void, glVertexAttrib4f, (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w), pfn(indx, x, y, z, w), return)
MACRO(PFNGLVERTEXATTRIB4FVPROC, void, glVertexAttrib4fv, (GLuint indx, const GLfloat* values), pfn(indx, values), return)
MACRO(PFNGLVERTEXATTRIBPOINTERPROC, void, glVertexAttribPointer, (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr), pfn(indx, size, type, normalized, stride, ptr), return)
//MACRO(PFNGLVIEWPORTPROC, void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height), pfn(x, y, width, height), return)

