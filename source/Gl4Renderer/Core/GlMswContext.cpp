#include "Core/GlMswContext.hpp"

#if ASHES_WIN32

#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Miscellaneous/GlDebug.hpp"

#include "ashesgl4_api.hpp"

#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
#include <gl/GL.h>

#include <iostream>
#include <sstream>

namespace ashes::gl4
{
	char const VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME[VK_MAX_EXTENSION_NAME_SIZE] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;

	namespace
	{
		template< typename FuncT >
		bool getFunction( char const * const name, FuncT & function )
		{
			function = reinterpret_cast< FuncT >( wglGetProcAddress( name ) );
			return function != nullptr;
		}

#if !defined( NDEBUG )

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS =  GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT | GL_CONTEXT_FLAG_DEBUG_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif
	}

	MswContext::MswContext( VkInstance instance
		, VkWin32SurfaceCreateInfoKHR createInfo
		, Context const * mainContext )
		: Context{ instance }
		, m_hDC{ ::GetDC( createInfo.hwnd ) }
		, m_hContext{ nullptr }
	{
		m_createInfo = std::move( createInfo );

		if ( doSelectFormat() )
		{
			m_hContext = wglCreateContext( m_hDC );
			enable();
			doLoadBaseFunctions();
			doLoadMswFunctions();
			doLoadDebugFunctions();
			disable();

			double fversion{ 0u };

			if ( !doCreateGl3Context( mainContext ) )
			{
				wglDeleteContext( m_hContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			enable();

			if ( m_wglSwapIntervalEXT )
			{
				m_wglSwapIntervalEXT( 0 );
			}

			disable();
		}
	}

	MswContext::MswContext( VkInstance instance
		, VkSurfaceKHR surface
		, Context const * mainContext )
		: MswContext
		{
			instance,
			get( surface )->getContext()->m_createInfo,
			mainContext
		}
	{
	}

	MswContext::~MswContext()
	{
		try
		{
			if ( m_hDC )
			{
				wglDeleteContext( m_hContext );
				::ReleaseDC( m_createInfo.hwnd, m_hDC );
			}
		}
		catch ( ... )
		{
		}
	}

	void MswContext::enable()const
	{
		wglMakeCurrent( m_hDC, m_hContext );
		m_enabled = true;
	}

	void MswContext::disable()const
	{
		m_enabled = false;
		wglMakeCurrent( nullptr, nullptr );
	}

	void MswContext::swapBuffers()const
	{
		::SwapBuffers( m_hDC );
	}

	void MswContext::doLoadBaseFunctions()
	{
#define GL_LIB_BASE_FUNCTION( fun )\
		m_gl##fun = &::gl##fun;
#define GL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
		}
#define GL_LIB_FUNCTION_OPT( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadMswFunctions()
	{
#	define WGL_LIB_BASE_FUNCTION( fun )\
		m_wgl##fun = &::wgl##fun;
#	define WGL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "wgl"#fun, m_wgl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "wgl"#fun };\
		}
#	define WGL_LIB_FUNCTION_OPT( fun )\
		if ( !( getFunction( "wgl"#fun, m_wgl##fun ) ) )\
		{\
			std::cerr << "Couldn't load function " << "gl"#fun << std::endl;\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadDebugFunctions()
	{
		auto & extensions = get( m_instance )->getExtensions();

		if ( extensions.find( KHR_debug ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( ARB_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", glDebugMessageCallback ) )
				{
					std::cerr << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( AMDX_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallbackAMD", glDebugMessageCallbackAMD ) )
			{
				std::cerr << "Unable to retrieve function glDebugMessageCallbackAMD" << std::endl;
			}
		}

		if ( glDebugMessageCallback )
		{
			if ( !getFunction( "glObjectLabel", glObjectLabel ) )
			{
				std::cerr << "Unable to retrieve function glObjectLabel" << std::endl;
			}

			if ( !getFunction( "glObjectPtrLabel", glObjectPtrLabel ) )
			{
				std::cerr << "Unable to retrieve function glObjectPtrLabel" << std::endl;
			}

			for ( auto & callback : get( m_instance )->getDebugCallbacks() )
			{
				glDebugMessageCallback( callback.callback, callback.userParam );
				::glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
		}
		else if ( glDebugMessageCallbackAMD )
		{
			for ( auto & callback : get( m_instance )->getDebugAMDCallbacks() )
			{
				glDebugMessageCallbackAMD( callback.callback, callback.userParam );
				::glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
		}
	}

	HGLRC MswContext::doCreateDummyContext()
	{
		HGLRC result = nullptr;

		if ( doSelectFormat() )
		{
			result = wglCreateContext( m_hDC );
		}

		return result;
	}

	bool MswContext::doSelectFormat()
	{
		bool result = false;
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cColorBits = 24;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int pixelFormats = ::ChoosePixelFormat( m_hDC, &pfd );

		if ( pixelFormats )
		{
			result = ::SetPixelFormat( m_hDC, pixelFormats, &pfd ) != FALSE;
		}

		return result;
	}

	bool MswContext::doCreateGl3Context( Context const * mainContext )
	{
		bool result = false;
		auto & extensions = get( m_instance )->getExtensions();

		try
		{
			using PFNGLCREATECONTEXTATTRIBS = HGLRC(*)( HDC hDC, HGLRC hShareContext, int const * attribList );
			PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
			HGLRC hContext = m_hContext;
			std::vector< int > attribList
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, extensions.getMajor(),
				WGL_CONTEXT_MINOR_VERSION_ARB, extensions.getMinor(),
				WGL_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
				WGL_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
				0
			};

			enable();
			::glGetError();
			getFunction( "wglCreateContextAttribsARB", glCreateContextAttribs );
			hContext = glCreateContextAttribs( m_hDC
				, ( mainContext
					? static_cast< MswContext const * >( mainContext )->m_hContext
					: nullptr )
				, attribList.data() );
			disable();
			wglDeleteContext( m_hContext );
			m_hContext = hContext;
			result = m_hContext != nullptr;

			if ( !result )
			{
				std::stringstream error;
				error << "Failed to create an OpenGL " << extensions.getMajor() << "." << extensions.getMinor() << " context (0x" << std::hex << ::glGetError() << ").";
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			result = false;
		}
		catch ( ... )
		{
			result = false;
		}

		return result;
	}
}

#endif
