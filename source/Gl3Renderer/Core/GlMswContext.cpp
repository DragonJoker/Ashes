#include "Core/GlMswContext.hpp"

#if defined( _WIN32 )

#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Miscellaneous/GlDebug.hpp"

#include <Ashes/Core/PlatformWindowHandle.hpp>

#include <Windows.h>
#include <GL/gl.h>

namespace gl_renderer
{
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

	MswContext::MswContext( Instance const & instance
		, ashes::WindowHandle const & handle
		, Context const * mainContext )
		: Context{ instance }
		, m_hDC( nullptr )
		, m_hContext( nullptr )
		, m_hWnd( handle.getInternal< ashes::IMswWindowHandle >().getHwnd() )
	{
		m_hDC = ::GetDC( m_hWnd );

		if ( doSelectFormat() )
		{
			m_hContext = wglCreateContext( m_hDC );
			enable();
			doLoadBaseFunctions();
			doLoadMswFunctions();
			doLoadDebugFunctions();
			disable();

			if ( !doCreateGl3Context( static_cast< MswContext const * >( mainContext ) ) )
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

	MswContext::~MswContext()
	{
		try
		{
			if ( m_hDC )
			{
				wglDeleteContext( m_hContext );
				::ReleaseDC( m_hWnd, m_hDC );
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
#define GL_LIB_FUNCTION_EXT( fun, ext, name )\
		if ( !( getFunction( "gl"#fun, m_gl##fun##_##ext ) ) )\
		{\
			ashes::Logger::logError( std::string{ "Couldn't load function " } + "gl"#fun );\
		}
#define GL_LIB_FUNCTION_VSN( fun, version )\
		if ( !( getFunction( "gl"#fun, m_gl##fun##_##version ) ) )\
		{\
			ashes::Logger::logError( std::string{ "Couldn't load function " } + "gl"#fun );\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadMswFunctions()
	{
#	define WGL_LIB_FUNCTION( fun )\
		if ( !( getFunction( "wgl"#fun, m_wgl##fun ) ) )\
		{\
			throw std::runtime_error{ std::string{ "Couldn't load function " } + "wgl"#fun };\
		}
#	define WGL_LIB_FUNCTION_EXT( fun, ext, name )\
		if ( !( getFunction( "wgl"#fun, m_wgl##fun##_##ext ) ) )\
		{\
			ashes::Logger::logError( std::string{ "Couldn't load function " } + "wgl"#fun );\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadDebugFunctions()
	{
		if ( m_instance.getExtensions().find( KHR_debug ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", glDebugMessageCallback ) )
				{
					ashes::Logger::logWarning( "Unable to retrieve function glDebugMessageCallback" );
				}
			}
		}
		else if ( m_instance.getExtensions().find( ARB_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", glDebugMessageCallback ) )
				{
					ashes::Logger::logWarning( "Unable to retrieve function glDebugMessageCallback" );
				}
			}
		}
		else if ( m_instance.getExtensions().find( AMDX_debug_output ) )
		{
			if ( !getFunction( "glDebugMessageCallbackAMD", glDebugMessageCallbackAMD ) )
			{
				ashes::Logger::logWarning( "Unable to retrieve function glDebugMessageCallbackAMD" );
			}
		}

		if ( glDebugMessageCallback )
		{
			if ( !getFunction( "glObjectLabel", glObjectLabel ) )
			{
				ashes::Logger::logWarning( "Unable to retrieve function glObjectLabel" );
			}

			if ( !getFunction( "glObjectPtrLabel", glObjectPtrLabel ) )
			{
				ashes::Logger::logWarning( "Unable to retrieve function glObjectPtrLabel" );
			}

			for ( auto & callback : m_instance.getDebugCallbacks() )
			{
				glDebugMessageCallback( callback.callback, callback.userParam );
				::glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
		}
		else if ( glDebugMessageCallbackAMD )
		{
			for ( auto & callback : m_instance.getDebugAMDCallbacks() )
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

		int pixelFormat = ::ChoosePixelFormat( m_hDC, &pfd );

		if ( pixelFormat )
		{
			result = ::SetPixelFormat( m_hDC, pixelFormat, &pfd ) != FALSE;
		}

		return result;
	}

	bool MswContext::doCreateGl3Context( MswContext const * mainContext )
	{
		bool result = false;

		try
		{
			using PFNGLCREATECONTEXTATTRIBS = HGLRC(*)( HDC hDC, HGLRC hShareContext, int const * attribList );
			PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
			HGLRC hContext = m_hContext;
			std::vector< int > attribList
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
				WGL_CONTEXT_MINOR_VERSION_ARB, 2,
				WGL_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
				WGL_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
				0
			};

			enable();
			::glGetError();
			getFunction( "wglCreateContextAttribsARB", glCreateContextAttribs );
			hContext = glCreateContextAttribs( m_hDC
				, ( mainContext
					? mainContext->m_hContext
					: nullptr )
				, attribList.data() );
			disable();
			wglDeleteContext( m_hContext );
			m_hContext = hContext;
			result = m_hContext != nullptr;

			if ( !result )
			{
				std::stringstream error;
				error << "Failed to create an OpenGL 3.2 context (0x" << std::hex << ::glGetError() << ").";
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( exc.what() );
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
