/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_OpenGLDefines_HPP___
#define ___GlRenderer_OpenGLDefines_HPP___
#pragma once

#include <cstdint>

#if RENDERLIB_WIN32
#	define GLAPIENTRY __stdcall
#	include <Windows.h>
#else
#	define GLAPIENTRY
#endif

namespace gl_renderer
{
	using GLbitfield = unsigned int;
	using GLboolean = unsigned char;
	using GLbyte = signed char;
	using GLchar = char;
	using GLclampd = double;
	using GLclampf = float;
	using GLdouble = double;
	using GLenum = unsigned int;
	using GLfloat = float;
	using GLint = int;
	using GLint64 = int64_t;
	using GLintptr = ptrdiff_t;
	using GLshort = short;
	using GLsizei = int;
	using GLsizeiptr = ptrdiff_t;
	using GLubyte = unsigned char;
	using GLuint = unsigned int;
	using GLuint64 = uint64_t;
	using GLulong = unsigned long;
	using GLushort = unsigned short;
	using GLvoid = void;
	typedef struct __GLsync *GLsync;

	static GLuint constexpr GL_INVALID_INDEX = ~GLuint( 0u );
	static GLuint constexpr GL_TEXTURE0 = 0x84C0;
	static GLuint constexpr GL_NO_ERROR = 0;
	static GLboolean constexpr GL_TRUE = 1;
	static GLboolean constexpr GL_FALSE = 0;

	enum Tweak
	{
		GL_CULL_FACE = 0x0B44,
		GL_DEPTH_TEST = 0x0B71,
		GL_STENCIL_TEST = 0x0B90,
		GL_BLEND = 0x0BE2,
		GL_POLYGON_OFFSET_POINT = 0x2A01,
		GL_POLYGON_OFFSET_LINE = 0x2A02,
		GL_MULTISAMPLE = 0x809D,
		GL_SAMPLE_ALPHA_TO_COVERAGE = 0x809E,
		GL_SAMPLE_ALPHA_TO_ONE = 0x809F,
		GL_POLYGON_OFFSET_FILL = 0x8037,
		GL_DEPTH_CLAMP = 0x864F,
		GL_RASTERIZER_DISCARD = 0x8C89,
	};

	enum PatchParameter
	{
		GL_PATCH_VERTICES = 0x8E72,
	};

	enum BaseType
	{
		GL_BYTE = 0x1400,
		GL_UNSIGNED_BYTE = 0x1401,
		GL_SHORT = 0x1402,
		GL_UNSIGNED_SHORT = 0x1403,
		GL_INT = 0x1404,
		GL_UNSIGNED_INT = 0x1405,
		GL_FLOAT = 0x1406,
	};

	enum ContextFlag
	{
		GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x0001,
		GL_CONTEXT_FLAG_DEBUG_BIT = 0x0002,
	};

	enum ContextMaskFlag
	{
		GL_CONTEXT_CORE_PROFILE_BIT = 0x00000001,
	};

	enum InformationType
	{
		GL_VENDOR = 0x1F00,
		GL_RENDERER = 0x1F01,
		GL_VERSION = 0x1F02,
		GL_EXTENSIONS = 0x1F03,
	};

	enum ClipOrigin
	{
		GL_LOWER_LEFT = 0x8CA1,
		GL_UPPER_LEFT = 0x8CA2,
	};

	enum ClipDepth
	{
		GL_NEGATIVE_ONE_TO_ONE = 0x935E,
		GL_ZERO_TO_ONE = 0x935F,
	};

#if RENDERLIB_WIN32
	using PFN_wglCreateContext = HGLRC ( GLAPIENTRY * )( HDC );
	using PFN_wglDeleteContext = BOOL ( GLAPIENTRY * )( HGLRC );
	using PFN_wglMakeCurrent = BOOL ( GLAPIENTRY * )( HDC, HGLRC );
	using PFN_wglSwapIntervalEXT = BOOL( GLAPIENTRY * )( int interval );

	enum ContextParameter
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091,
		WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092,
		WGL_CONTEXT_FLAGS_ARB = 0x2094,
		WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126,
	};
#elif RENDERLIB_XLIB
#else
#endif

	using PFN_glActiveTexture = void ( GLAPIENTRY * ) ( GLenum texture );
	using PFN_glAttachShader = void ( GLAPIENTRY * ) ( GLuint program, GLuint shader );
	using PFN_glBeginQuery = void ( GLAPIENTRY * )( GLenum target, GLuint id );
	using PFN_glBindBuffer = void ( GLAPIENTRY * )( GLenum target, GLuint buffer );
	using PFN_glBindBufferBase = void ( GLAPIENTRY * ) ( GLenum target, GLuint index, GLuint buffer );
	using PFN_glBindFramebuffer = void ( GLAPIENTRY * ) ( GLenum target, GLuint framebuffer );
	using PFN_glBindImageTexture = void ( GLAPIENTRY * ) ( GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format );
	using PFN_glBindSampler = void ( GLAPIENTRY * )( GLuint unit, GLuint sampler );
	using PFN_glBindTexture = void ( GLAPIENTRY * )( GLenum target, GLuint texture );
	using PFN_glBindVertexArray = void ( GLAPIENTRY * ) ( GLuint array );
	using PFN_glBlendColor = void ( GLAPIENTRY * ) ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
	using PFN_glBlendEquationSeparatei = void ( GLAPIENTRY * ) ( GLuint buf, GLenum modeRGB, GLenum modeAlpha );
	using PFN_glBlendFuncSeparatei = void ( GLAPIENTRY * ) ( GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha );
	using PFN_glBufferStorage = void ( GLAPIENTRY * )( GLenum target, GLsizeiptr size, const void * data, GLbitfield flags );
	using PFN_glCheckFramebufferStatus = GLenum( GLAPIENTRY * ) ( GLenum target );
	using PFN_glClearBufferfi = void ( GLAPIENTRY * ) ( GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil );
	using PFN_glClearBufferfv = void ( GLAPIENTRY * ) ( GLenum buffer, GLint drawBuffer, const GLfloat* value );
	using PFN_glClearBufferiv = void ( GLAPIENTRY * ) ( GLenum buffer, GLint drawBuffer, const GLint* value );
	using PFN_glClearBufferuiv = void ( GLAPIENTRY * ) ( GLenum buffer, GLint drawBuffer, const GLuint* value );
	using PFN_glClearTexImage = void ( GLAPIENTRY * ) ( GLuint texture, GLint level, GLenum format, GLenum type, const void *data );
	using PFN_glClientWaitSync = GLenum( GLAPIENTRY * ) ( GLsync GLsync, GLbitfield flags, GLuint64 timeout );
	using PFN_glClipControl = void ( GLAPIENTRY * )( GLenum origin, GLenum depth );
	using PFN_glCompileShader = void ( GLAPIENTRY * ) ( GLuint shader );
	using PFN_glCopyBufferSubData = void ( GLAPIENTRY * ) ( GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size );
	using PFN_glCopyImageSubData = void ( GLAPIENTRY * ) ( GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth );
	using PFN_glCreateProgram = GLuint( GLAPIENTRY * ) ( void );
	using PFN_glCreateShader = GLuint( GLAPIENTRY * ) ( GLenum type );
	using PFN_glCreateVertexArrays = void ( GLAPIENTRY * ) ( GLsizei n, GLuint* arrays );
	using PFN_glCullFace = void ( GLAPIENTRY * )( GLenum mode );
	using PFN_glDeleteBuffers = void ( GLAPIENTRY * )( GLsizei n, const GLuint * buffers );
	using PFN_glDeleteFramebuffers = void ( GLAPIENTRY * ) ( GLsizei n, const GLuint* framebuffers );
	using PFN_glDeleteProgram = void ( GLAPIENTRY * ) ( GLuint program );
	using PFN_glDeleteQueries = void ( GLAPIENTRY * )( GLsizei n, const GLuint * ids );
	using PFN_glDeleteSamplers = void ( GLAPIENTRY * )( GLsizei count, const GLuint * samplers );
	using PFN_glDeleteShader = void ( GLAPIENTRY * ) ( GLuint shader );
	using PFN_glDeleteSync = void ( GLAPIENTRY * ) ( GLsync GLsync );
	using PFN_glDeleteTextures = void ( GLAPIENTRY * )( GLsizei n, GLuint const * textures );
	using PFN_glDeleteVertexArrays = void ( GLAPIENTRY * ) ( GLsizei n, const GLuint* arrays );
	using PFN_glDepthFunc = void ( GLAPIENTRY * )( GLenum func );
	using PFN_glDepthMask = void ( GLAPIENTRY * )( GLboolean flag );
	using PFN_glDepthRange = void ( GLAPIENTRY * )( GLclampd zNear, GLclampd zFar );
	using PFN_glDisable = void ( GLAPIENTRY * )( GLenum cap );
	using PFN_glDrawArrays = void ( GLAPIENTRY * )( GLenum mode, GLint first, GLsizei count );
	using PFN_glDrawArraysInstancedBaseInstance = void ( GLAPIENTRY * ) ( GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance );
	using PFN_glDrawBuffers = void ( GLAPIENTRY * ) ( GLsizei n, const GLenum* bufs );
	using PFN_glDrawElementsBaseVertex = void ( GLAPIENTRY * ) ( GLenum mode, GLsizei count, GLenum type, void *indices, GLint basevertex );
	using PFN_glDrawElementsInstancedBaseInstance = void ( GLAPIENTRY * ) ( GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLuint baseinstance );
	using PFN_glEnable = void ( GLAPIENTRY * )( GLenum cap );
	using PFN_glEnableVertexAttribArray = void ( GLAPIENTRY * ) ( GLuint index );
	using PFN_glEndQuery = void ( GLAPIENTRY * )( GLenum target );
	using PFN_glFenceSync = GLsync( GLAPIENTRY * ) ( GLenum condition, GLbitfield flags );
	using PFN_glFinish = void ( GLAPIENTRY * )( void );
	using PFN_glFramebufferTexture1D = void ( GLAPIENTRY * ) ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	using PFN_glFramebufferTexture2D = void ( GLAPIENTRY * ) ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	using PFN_glFramebufferTexture3D = void ( GLAPIENTRY * ) ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer );
	using PFN_glFramebufferTextureLayer = void ( GLAPIENTRY * ) ( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer );
	using PFN_glFrontFace = void ( GLAPIENTRY * )( GLenum mode );
	using PFN_glGenBuffers = void ( GLAPIENTRY * )( GLsizei n, GLuint * buffers );
	using PFN_glGenFramebuffers = void ( GLAPIENTRY * ) ( GLsizei n, GLuint* framebuffers );
	using PFN_glGenQueries = void ( GLAPIENTRY * )( GLsizei n, GLuint * ids );
	using PFN_glGenSamplers = void ( GLAPIENTRY * )( GLsizei count, GLuint * samplers );
	using PFN_glGenTextures = void ( GLAPIENTRY * )( GLsizei n, GLuint * textures );
	using PFN_glGenerateMipmap = void ( GLAPIENTRY * )( GLenum target );
	using PFN_glGetError = GLenum( GLAPIENTRY * )( void );
	using PFN_glGetProgramInfoLog = void ( GLAPIENTRY * ) ( GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog );
	using PFN_glGetProgramiv = void ( GLAPIENTRY * ) ( GLuint program, GLenum pname, GLint* param );
	using PFN_glGetQueryObjectui64v = void ( GLAPIENTRY * )( GLuint id, GLenum pname, GLuint64 * params );
	using PFN_glGetQueryObjectuiv = void ( GLAPIENTRY * )( GLuint id, GLenum pname, GLuint * params );
	using PFN_glGetShaderInfoLog = void ( GLAPIENTRY * ) ( GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog );
	using PFN_glGetShaderiv = void ( GLAPIENTRY * ) ( GLuint shader, GLenum pname, GLint* param );
	using PFN_glGetString = GLubyte *( GLAPIENTRY * )( GLenum name ); 
	using PFN_glGetTexImage = void ( GLAPIENTRY * )( GLenum target, GLint level, GLenum format, GLenum type, void *pixels );
	using PFN_glLineWidth = void ( GLAPIENTRY * )( GLfloat width );
	using PFN_glLinkProgram = void ( GLAPIENTRY * ) ( GLuint program );
	using PFN_glLogicOp = void ( GLAPIENTRY * )( GLenum opcode );
	using PFN_glMapBufferRange = void * ( GLAPIENTRY * )( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access );
	using PFN_glPatchParameteri = void ( GLAPIENTRY * ) ( GLenum pname, GLint value );
	using PFN_glPolygonMode = void ( GLAPIENTRY * )( GLenum face, GLenum mode );
	using PFN_glPolygonOffsetClampEXT = void ( GLAPIENTRY * ) ( GLfloat factor, GLfloat units, GLfloat clamp );
	using PFN_glQueryCounter = void ( GLAPIENTRY * )( GLuint id, GLenum target );
	using PFN_glReadBuffer = void ( GLAPIENTRY * )( GLenum mode );
	using PFN_glReadPixels = void( GLAPIENTRY * )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels );
	using PFN_glSamplerParameterf = void ( GLAPIENTRY * )( GLuint sampler, GLenum pname, GLfloat param );
	using PFN_glSamplerParameterfv = void ( GLAPIENTRY * )( GLuint sampler, GLenum pname, const GLfloat * param );
	using PFN_glSamplerParameteri = void ( GLAPIENTRY * )( GLuint sampler, GLenum pname, GLint param );
	using PFN_glSamplerParameteriv = void ( GLAPIENTRY * )( GLuint sampler, GLenum pname, const GLint * param );
	using PFN_glScissor = void ( GLAPIENTRY * )( GLint x, GLint y, GLsizei width, GLsizei height );
	using PFN_glShaderSource = void ( GLAPIENTRY * ) ( GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length );
	using PFN_glStencilFuncSeparate = void ( GLAPIENTRY * ) ( GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask );
	using PFN_glStencilMaskSeparate = void ( GLAPIENTRY * ) ( GLenum face, GLuint mask );
	using PFN_glStencilOpSeparate = void ( GLAPIENTRY * ) ( GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass );
	using PFN_glTexBufferRange = void ( GLAPIENTRY * ) ( GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size );
	using PFN_glTexImage1D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels );
	using PFN_glTexImage2D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels );
	using PFN_glTexImage2DMultisample = void ( GLAPIENTRY * )( GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations );
	using PFN_glTexImage3D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels );
	using PFN_glTexImage3DMultisample = void ( GLAPIENTRY * )( GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations );
	using PFN_glTexSubImage1D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels );
	using PFN_glTexSubImage2D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels );
	using PFN_glTexSubImage3D = void ( GLAPIENTRY * )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels );
	using PFN_glUnmapBuffer = GLboolean( GLAPIENTRY * )( GLenum target );
	using PFN_glUseProgram = void ( GLAPIENTRY * ) ( GLuint program );
	using PFN_glVertexAttribPointer = void ( GLAPIENTRY * ) ( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer );
	using PFN_glViewport = void ( GLAPIENTRY * )( GLint x, GLint y, GLsizei width, GLsizei height );
}

#endif
