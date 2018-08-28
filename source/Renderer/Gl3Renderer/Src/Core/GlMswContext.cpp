#include "Core/GlMswContext.hpp"

#if defined( _WIN32 )

#include "Core/GlPhysicalDevice.hpp"
#include "Miscellaneous/GlDebug.hpp"

#include <Core/PlatformWindowHandle.hpp>

#include <Windows.h>
#include <wingdi.h>
#undef MemoryBarrier

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

	MswContext::MswContext( PhysicalDevice const & gpu
		, renderer::ConnectionPtr && connection )
		: Context{ gpu, std::move( connection ) }
		, m_hDC( nullptr )
		, m_hContext( nullptr )
		, m_hWnd( m_connection->getHandle().getInternal< renderer::IMswWindowHandle >().getHwnd() )
	{
		m_hDC = ::GetDC( m_hWnd );

		if ( doSelectFormat() )
		{
			m_hContext = wglCreateContext( m_hDC );
			setCurrent();
			doLoadBaseFunctions();
			doLoadMswFunctions();
			loadDebugFunctions();
			endCurrent();

			double fversion{ 0u };

			if ( !doCreateGl3Context() )
			{
				wglDeleteContext( m_hContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			setCurrent();
			wglSwapIntervalEXT( 0 );
			endCurrent();
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

	void MswContext::setCurrent()const
	{
		wglMakeCurrent( m_hDC, m_hContext );
	}

	void MswContext::endCurrent()const
	{
		wglMakeCurrent( nullptr, nullptr );
	}

	void MswContext::swapBuffers()const
	{
		::SwapBuffers( m_hDC );
	}

	void MswContext::doLoadBaseFunctions()
	{
#define GL_LIB_BASE_FUNCTION( fun )\
			this->gl##fun = &::gl##fun;
#define GL_LIB_FUNCTION( fun )\
			if ( !( getFunction( "gl"#fun, this->gl##fun ) ) )\
			{\
				throw std::runtime_error{ std::string{ "Couldn't load function " } + "gl"#fun };\
			}
#define GL_LIB_FUNCTION_EXT( fun, ext, name )\
			if ( !( getFunction( "gl"#fun, this->gl##fun##_##ext ) ) )\
			{\
				renderer::Logger::logError( std::string{ "Couldn't load function " } + "gl"#fun );\
			}
#define GL_LIB_FUNCTION_VSN( fun, version )\
			if ( !( getFunction( "gl"#fun, this->gl##fun##_##version ) ) )\
			{\
				renderer::Logger::logError( std::string{ "Couldn't load function " } + "gl"#fun );\
			}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadMswFunctions()
	{
#	define WGL_LIB_FUNCTION( fun )\
			if ( !( getFunction( "wgl"#fun, this->wgl##fun ) ) )\
			{\
				throw std::runtime_error{ std::string{ "Couldn't load function " } + "wgl"#fun };\
			}
#	define WGL_LIB_FUNCTION_EXT( fun, ext, name )\
			if ( !( getFunction( "wgl"#fun, this->wgl##fun##_##ext ) ) )\
			{\
				renderer::Logger::logError( std::string{ "Couldn't load function " } + "wgl"#fun );\
			}
#include "Miscellaneous/OpenGLFunctionsList.inl"
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

	bool MswContext::doCreateGl3Context()
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

			setCurrent();
			glGetError();
			glCreateContextAttribs = ( PFNGLCREATECONTEXTATTRIBS )wglGetProcAddress( "wglCreateContextAttribsARB" );
			hContext = glCreateContextAttribs( m_hDC, nullptr, attribList.data() );
			endCurrent();
			wglDeleteContext( m_hContext );
			m_hContext = hContext;
			result = m_hContext != nullptr;

			if ( !result )
			{
				std::stringstream error;
				error << "Failed to create an OpenGL 3.2 context (0x" << std::hex << glGetError() << ").";
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( ... )
		{
			result = false;
		}

		return result;
	}
}

#endif
