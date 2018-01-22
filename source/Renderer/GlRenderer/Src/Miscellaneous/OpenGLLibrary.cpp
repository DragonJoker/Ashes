/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Miscellaneous/OpenGLLibrary.hpp"

#include <GL/GL.h>

namespace gl_renderer
{
	namespace
	{
#if RENDERLIB_WIN32
		template< typename FuncT >
		bool getFunction( char const * const name, FuncT & function )
		{
			function = ( FuncT )wglGetProcAddress( name );
			return function != nullptr;
		}
#elif RENDERLIB_XLIB
#else
#endif
	}

	OpenGLLibrary::OpenGLLibrary()
	{
		try
		{
#define GL_LIB_BASE_FUNCTION( fun )\
			gl::fun = ( PFN_gl##fun )&gl##fun;

#define GL_LIB_FUNCTION( fun )\
			if ( !( getFunction( "gl"#fun, gl::fun ) ) )\
			{\
				throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
			}
#include "OpenGLFunctionsList.inl"

#if RENDERLIB_WIN32
#	define WGL_LIB_BASE_FUNCTION( fun )\
			wgl::fun = &wgl##fun;
#	define WGL_LIB_FUNCTION( fun )\
			if ( !( getFunction( "wgl"#fun, wgl::fun ) ) )\
			{\
				throw std::runtime_error{ std::string{ "Couldn't load function " } + "wgl"#fun };\
			}
#	include "OpenGLFunctionsList.inl"
#elif RENDERLIB_XLIB
#else
#endif
		}
		catch ( std::exception & error )
		{
			std::cerr << error.what() << std::endl;
		}
	}
}
