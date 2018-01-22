#include "Miscellaneous/GlDebug.hpp"

#include <cstdint>
#include <iostream>
#include <sstream>

#if RENDERLIB_WIN32
#   include <Windows.h>
#elif RENDERLIB_XLIB
#endif

#include <GL/GL.h>

//*************************************************************************************************

namespace gl_api
{
	/**
	*\brief
	*	Récupère une fonction dans la dll OpenGL actuellement chargée
	*\param[in] p_strName
	*	Le nom de la fonction
	*\param[in] p_func
	*	Reçoit la fonction
	*\return
	*	true si la fonction a été trouvée, false sinon
	*/
	template< typename Func >
	bool getFunction( std::string const & p_strName, Func & p_func )
	{
#if RENDERLIB_WIN32
		p_func = reinterpret_cast< Func >( wglGetProcAddress( p_strName.c_str() ) );
#elif RENDERLIB_XLIB
		p_func = reinterpret_cast< Func >( glXGetProcAddress( ( GLubyte const * )p_strName.c_str() ) );
#endif
		return p_func != NULL;
	}

#define makeGlExtension( x )	static const std::string x = "GL_"#x;

	makeGlExtension( ARB_debug_output )
	makeGlExtension( AMDX_debug_output )
}

//*************************************************************************************************

namespace gl_renderer
{
	enum GlDebugOutput
		: GLenum
	{
		GL_DEBUG_OUTPUT_SYNCHRONOUS = 0x8242,
	};
	enum GlDebugSource
		: GLenum
	{
		GL_DEBUG_SOURCE_API = 0x8246,
		GL_DEBUG_SOURCE_WINDOW_SYSTEM = 0x8247,
		GL_DEBUG_SOURCE_SHADER_COMPILER = 0x8248,
		GL_DEBUG_SOURCE_THIRD_PARTY = 0x8249,
		GL_DEBUG_SOURCE_APPLICATION = 0x824A,
		GL_DEBUG_SOURCE_OTHER = 0x824B,
	};
	enum GlDebugType
	{
		GL_DEBUG_TYPE_ERROR = 0x824C,
		GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D,
		GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR = 0x824E,
		GL_DEBUG_TYPE_PORTABILITY = 0x824F,
		GL_DEBUG_TYPE_PERFORMANCE = 0x8250,
		GL_DEBUG_TYPE_OTHER = 0x8251,
		GL_DEBUG_TYPE_MARKER = 0x8268,
		GL_DEBUG_TYPE_PUSH_GROUP = 0x8269,
		GL_DEBUG_TYPE_POP_GROUP = 0x826A,
	};
	enum GlDebugSeverity
	{
		GL_DEBUG_SEVERITY_NOTIFICATION = 0x826B,
		GL_DEBUG_SEVERITY_HIGH = 0x9146,
		GL_DEBUG_SEVERITY_MEDIUM = 0x9147,
		GL_DEBUG_SEVERITY_LOW = 0x9148,
	};
	enum GlDebugCategory
	{
		GL_DEBUG_CATEGORY_API_ERROR_AMD = 0x9149,
		GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD = 0x914A,
		GL_DEBUG_CATEGORY_DEPRECATION_AMD = 0x914B,
		GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD = 0x914C,
		GL_DEBUG_CATEGORY_PERFORMANCE_AMD = 0x914D,
		GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD = 0x914E,
		GL_DEBUG_CATEGORY_APPLICATION_AMD = 0x914F,
		GL_DEBUG_CATEGORY_OTHER_AMD = 0x9150,
	};

	using PFNGLDEBUGPROC = void ( CALLBACK * )( uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char * message, void * userParam );
	using PFNGLDEBUGAMDPROC = void ( CALLBACK * )( uint32_t id, uint32_t category, uint32_t severity, int length, const char* message, void* userParam );
	using PFNGLDEBUGMESSAGECALLBACK = void ( CALLBACK * )( PFNGLDEBUGPROC callback, void * userParam );
	using PFNGLDEBUGMESSAGECALLBACKAMD = void ( CALLBACK * )( PFNGLDEBUGAMDPROC callback, void * userParam );

	PFNGLDEBUGMESSAGECALLBACK glDebugMessageCallback = nullptr;
	PFNGLDEBUGMESSAGECALLBACKAMD glDebugMessageCallbackAMD = nullptr;

	void loadDebugFunctions()
	{
		auto ext = ( char const * )glGetString( GL_EXTENSIONS );
		std::string extensions = ext ? ext : "";

		if ( extensions.find( gl_api::ARB_debug_output ) != std::string::npos )
		{
			if ( !gl_api::getFunction( "glDebugMessageCallback", glDebugMessageCallback ) )
			{
				if ( !gl_api::getFunction( "glDebugMessageCallbackARB", glDebugMessageCallback ) )
				{
					std::cout << "Unable to retrieve function glDebugMessageCallback" << std::endl;
				}
			}
		}
		else if ( extensions.find( gl_api::AMDX_debug_output ) != std::string::npos )
		{
			if ( !gl_api::getFunction( "glDebugMessageCallbackAMD", glDebugMessageCallbackAMD ) )
			{
				std::cout << "Unable to retrieve function glDebugMessageCallbackAMD" << std::endl;
			}
		}
	}

	void initialiseDebugFunctions()
	{
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
	}

	void CALLBACK callbackDebugLog( uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char * message, void * userParam )
	{
		std::stringstream stream;
		stream << "OpenGL Debug\n";

		switch ( source )
		{
		case GL_DEBUG_SOURCE_API:				stream << "    Source: OpenGL\n";			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		stream << "    Source: Windows\n";			break;
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

		stream << "    ID: " << id << "\n";

		switch ( severity )
		{
		case GL_DEBUG_SEVERITY_HIGH:			stream << "    Severity: High\n";			break;
		case GL_DEBUG_SEVERITY_MEDIUM:			stream << "    Severity: Medium\n";			break;
		case GL_DEBUG_SEVERITY_LOW:				stream << "    Severity: Low\n";			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	stream << "    Severity: Notification\n";	break;
		}

		stream << "    Message: " << message;

		if ( severity != GL_DEBUG_SEVERITY_NOTIFICATION )
		{
			std::cout << stream.str() << std::endl;
		}
	}

	void CALLBACK callbackDebugLogAMD( uint32_t id, uint32_t category, uint32_t severity, int length, const char* message, void* userParam )
	{
		std::stringstream stream;
		stream << "OpenGL Debug\n";

		switch ( category )
		{
		case GL_DEBUG_CATEGORY_API_ERROR_AMD:			stream << "Category: OpenGL\n";					break;
		case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:		stream << "Category: Windows\n";				break;
		case GL_DEBUG_CATEGORY_DEPRECATION_AMD:			stream << "Category: Deprecated behavior\n";	break;
		case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:	stream << "Category: Undefined behavior\n";		break;
		case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:			stream << "Category: Performance\n";			break;
		case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:		stream << "Category: Shader compiler\n";		break;
		case GL_DEBUG_CATEGORY_APPLICATION_AMD:			stream << "Category: Application\n";			break;
		case GL_DEBUG_CATEGORY_OTHER_AMD:				stream << "Category: Other\n";					break;
		}

		stream << "    ID: " << id << "\n";

		switch ( severity )
		{
		case GL_DEBUG_SEVERITY_HIGH:					stream << "Severity: High\n";					break;
		case GL_DEBUG_SEVERITY_MEDIUM:					stream << "Severity: Medium\n";					break;
		case GL_DEBUG_SEVERITY_LOW:						stream << "Severity: Low\n";					break;
		}

		stream << "    Message: " << message;
		std::cout << stream.str() << std::endl;
	}

	//*************************************************************************************************
}
