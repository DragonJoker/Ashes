/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	namespace gl
	{
#define GL_LIB_BASE_FUNCTION( fun ) PFN_gl##fun fun;
#define GL_LIB_FUNCTION( fun ) PFN_gl##fun fun;
#define GL_LIB_FUNCTION_EXT( fun, ext, name ) PFN_gl##fun fun##_##ext;
#define GL_LIB_FUNCTION_VSN( fun, version ) PFN_gl##fun fun##_##version;
#include "OpenGLFunctionsList.inl"
	}

#if RENDERLIB_WIN32
	namespace wgl
	{
#define WGL_LIB_BASE_FUNCTION( fun ) PFN_wgl##fun fun;
#define WGL_LIB_FUNCTION( fun ) PFN_wgl##fun fun;
#define WGL_LIB_FUNCTION_EXT( fun, ext, name ) PFN_wgl##fun fun##_##ext;
#include "OpenGLFunctionsList.inl"
	}
#elif RENDERLIB_XLIB
#else
#endif
}
