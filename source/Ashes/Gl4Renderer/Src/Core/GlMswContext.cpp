#include "Core/GlMswContext.hpp"

#if defined( _WIN32 )

#include "Core/GlPhysicalDevice.hpp"
#include "Miscellaneous/GlDebug.hpp"

#include <Core/PlatformWindowHandle.hpp>

#include <Windows.h>
#include <gl/GL.h>

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

#define makeGlExtension( x )	static const std::string x = "GL_"#x

		makeGlExtension( KHR_debug );
		makeGlExtension( ARB_debug_output );
		makeGlExtension( AMDX_debug_output );

		void GLAPIENTRY callbackDebugLog( uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char * message, void * userParam )
		{
			std::locale loc{ "C" };
			std::stringstream stream;
			stream.imbue( loc );
			stream << "OpenGL Debug\n";

			switch ( source )
			{
			case GL_DEBUG_SOURCE_API:				stream << "    Source: OpenGL\n";			break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		stream << "    Source: Window System\n";	break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	stream << "    Source: Shader compiler\n";	break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:		stream << "    Source: Third party\n";		break;
			case GL_DEBUG_SOURCE_APPLICATION:		stream << "    Source: Application\n";		break;
			case GL_DEBUG_SOURCE_OTHER:				stream << "    Source: Other\n";			break;
			}

			switch ( type )
			{
			case GL_DEBUG_TYPE_ERROR:				stream << "    Type: Error\n";					break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	stream << "    Type: Deprecated behavior\n";	break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	stream << "    Type: Undefined behavior\n";		break;
			case GL_DEBUG_TYPE_PORTABILITY:			stream << "    Type: Portability\n";			break;
			case GL_DEBUG_TYPE_PERFORMANCE:			stream << "    Type: Performance\n";			break;
			case GL_DEBUG_TYPE_OTHER:				stream << "    Type: Other\n";					break;
			}

			stream << "    ID: 0x" << std::hex << id << " (" << getErrorName( id, type ) << ")\n";

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:			stream << "    Severity: High\n";			break;
			case GL_DEBUG_SEVERITY_MEDIUM:			stream << "    Severity: Medium\n";			break;
			case GL_DEBUG_SEVERITY_LOW:				stream << "    Severity: Low\n";			break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:	stream << "    Severity: Notification\n";	break;
			}

			stream << "    Message: " << message;

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:
				ashes::Logger::logError( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				ashes::Logger::logWarning( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_LOW:
				ashes::Logger::logInfo( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				break;
			default:
				ashes::Logger::logDebug( stream.str() );
				break;
			}
	}

		void GLAPIENTRY callbackDebugLogAMD( uint32_t id, uint32_t category, uint32_t severity, int length, const char* message, void* userParam )
		{
			std::locale loc{ "C" };
			std::stringstream stream;
			stream.imbue( loc );
			stream << "OpenGL Debug\n";

			switch ( category )
			{
			case GL_DEBUG_CATEGORY_API_ERROR_AMD:			stream << "    Category: OpenGL\n";					break;
			case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:		stream << "    Category: Window System\n";			break;
			case GL_DEBUG_CATEGORY_DEPRECATION_AMD:			stream << "    Category: Deprecated behavior\n";	break;
			case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:	stream << "    Category: Undefined behavior\n";		break;
			case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:			stream << "    Category: Performance\n";			break;
			case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:		stream << "    Category: Shader compiler\n";		break;
			case GL_DEBUG_CATEGORY_APPLICATION_AMD:			stream << "    Category: Application\n";			break;
			case GL_DEBUG_CATEGORY_OTHER_AMD:				stream << "    Category: Other\n";					break;
			}

			stream << "    ID: 0x" << std::hex << id << " (" << getErrorName( id, category ) << ")\n";

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:					stream << "    Severity: High\n";					break;
			case GL_DEBUG_SEVERITY_MEDIUM:					stream << "    Severity: Medium\n";					break;
			case GL_DEBUG_SEVERITY_LOW:						stream << "    Severity: Low\n";					break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:			stream << "    Severity: Notification\n";			break;
			}

			stream << "    Message: " << message;

			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:
				ashes::Logger::logError( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				ashes::Logger::logWarning( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_LOW:
				ashes::Logger::logInfo( stream.str() );
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				break;
			default:
				ashes::Logger::logDebug( stream.str() );
				break;
			}
		}

#else

		static const int GL_CONTEXT_CREATION_DEFAULT_FLAGS = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
		static const int GL_CONTEXT_CREATION_DEFAULT_MASK = GL_CONTEXT_CORE_PROFILE_BIT;

#endif
	}

	MswContext::MswContext( PhysicalDevice const & gpu
		, ashes::Connection const & connection
		, Context const * mainContext )
		: Context{ gpu, connection }
		, m_hDC( nullptr )
		, m_hContext( nullptr )
		, m_hWnd( m_connection.getHandle().getInternal< ashes::IMswWindowHandle >().getHwnd() )
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

			double fversion{ 0u };

			if ( !doCreateGl3Context( mainContext ) )
			{
				wglDeleteContext( m_hContext );
				throw std::runtime_error{ "The supported OpenGL version is insufficient." };
			}

			enable();
			m_wglSwapIntervalEXT( 0 );

#if !defined( NDEBUG )
			if ( glDebugMessageCallback )
			{
				glDebugMessageCallback( PFNGLDEBUGPROC( &callbackDebugLog ), nullptr );
				glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
			else if ( glDebugMessageCallbackAMD )
			{
				glDebugMessageCallbackAMD( PFNGLDEBUGAMDPROC( &callbackDebugLogAMD ), nullptr );
				glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			}
#endif

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
#define GL_LIB_FUNCTION_OPT( fun )\
		if ( !( getFunction( "gl"#fun, m_gl##fun ) ) )\
		{\
			ashes::Logger::logError( std::string{ "Couldn't load function " } + "gl"#fun );\
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
			ashes::Logger::logError( std::string{ "Couldn't load function " } + "wgl"#fun );\
		}
#include "Miscellaneous/OpenGLFunctionsList.inl"
	}

	void MswContext::doLoadDebugFunctions()
	{
#if !defined( NDEBUG )
		auto ext = ( char const * )::glGetString( GL_EXTENSIONS );
		std::string extensions = ext ? ext : "";

		if ( extensions.find( KHR_debug ) != std::string::npos )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackKHR", glDebugMessageCallback ) )
				{
					ashes::Logger::logWarning( "Unable to retrieve function glDebugMessageCallback" );
				}
			}
		}
		else if ( extensions.find( ARB_debug_output ) != std::string::npos )
		{
			if ( !getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !getFunction( "glDebugMessageCallbackARB", glDebugMessageCallback ) )
				{
					ashes::Logger::logWarning( "Unable to retrieve function glDebugMessageCallback" );
				}
			}
		}
		else if ( extensions.find( AMDX_debug_output ) != std::string::npos )
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
		}
#endif
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

		try
		{
			using PFNGLCREATECONTEXTATTRIBS = HGLRC(*)( HDC hDC, HGLRC hShareContext, int const * attribList );
			PFNGLCREATECONTEXTATTRIBS glCreateContextAttribs;
			HGLRC hContext = m_hContext;
			std::vector< int > attribList
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, m_gpu.getMajor(),
				WGL_CONTEXT_MINOR_VERSION_ARB, m_gpu.getMinor(),
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
				error << "Failed to create an OpenGL " << m_gpu.getMajor() << "." << m_gpu.getMinor() << " context (0x" << std::hex << ::glGetError() << ").";
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
