/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Gl3Renderer/Core/GlDebugReportCallback.hpp"

#include "Gl3Renderer/Core/GlInstance.hpp"

#if ASHES_WIN32
#	include <Windows.h>
#	include <gl/GL.h>
#elif ASHES_XLIB
#	include <X11/Xlib.h>
#	include <GL/glx.h>
#endif

namespace gl_renderer
{
	namespace
	{
		void GLAPIENTRY callbackDebugLog( uint32_t source
			, uint32_t type
			, uint32_t id
			, uint32_t severity
			, int length
			, const char * message
			, DebugReportCallback * userParam )
		{
			userParam->report( GlDebugSource( source )
				, GlDebugType( type )
				, id
				, GlDebugSeverity( severity )
				, length
				, message );
		}


		void GLAPIENTRY callbackDebugLogAMD( uint32_t id
			, uint32_t category
			, uint32_t severity
			, int length
			, const char * message
			, DebugReportCallback * userParam )
		{
			userParam->report( id
				, GlDebugCategory( category )
				, GlDebugSeverity( severity )
				, length
				, message );
		}

		char const * const convert( GlDebugSource source )
		{
			switch ( source )
			{
			case GL_DEBUG_SOURCE_API:
				return "OpenGL";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "Window System";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "Shader compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "Third party";
			case GL_DEBUG_SOURCE_APPLICATION:
				return "Application";
			case GL_DEBUG_SOURCE_OTHER:
				return "Other";
			default:
				return "Unknown";
			}
		}

		char const * const convert( GlDebugCategory category )
		{
			switch ( category )
			{
			case GL_DEBUG_CATEGORY_API_ERROR_AMD:
				return "OpenGL";
			case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
				return "Window System";
			case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
				return "Deprecation";
			case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
				return "Undefined Behavior";
			case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
				return "Performance";
			case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
				return "Shader Compiler";
			case GL_DEBUG_CATEGORY_APPLICATION_AMD:
				return "Application";
			case GL_DEBUG_CATEGORY_OTHER_AMD:
				return "Other";
			default:
				return "Unknown";
			}
		}

		ashes::DebugReportFlags convert( GlDebugType type )
		{
			switch ( type )
			{
			case GL_DEBUG_TYPE_ERROR:
				return ashes::DebugReportFlag::eError;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			case GL_DEBUG_TYPE_PORTABILITY:
				return ashes::DebugReportFlag::eWarning;
			case GL_DEBUG_TYPE_PERFORMANCE:
				return ashes::DebugReportFlag::ePerformanceWarning;
			default:
				return ashes::DebugReportFlag::eDebug;
			}
		}

		ashes::DebugReportFlags convert( GlDebugSeverity severity )
		{
			switch ( severity )
			{
			case GL_DEBUG_SEVERITY_HIGH:
				return ashes::DebugReportFlag::eError;
			case GL_DEBUG_SEVERITY_MEDIUM:
				return ashes::DebugReportFlag::eWarning;
			case GL_DEBUG_SEVERITY_LOW:
				return ashes::DebugReportFlag::eDebug;
			default:
				return ashes::DebugReportFlag::eInformation;
			}
		}
	}

	DebugReportCallback::DebugReportCallback( Instance const & instance
		, ashes::DebugReportCallbackCreateInfo createInfo )
		: ashes::DebugReportCallback{ instance, std::move( createInfo ) }
		, m_instance{ instance }
	{
		m_instance.registerDebugMessageCallback( PFNGLDEBUGPROC( &callbackDebugLog ), this );
		m_instance.registerDebugMessageCallbackAMD( PFNGLDEBUGAMDPROC( &callbackDebugLogAMD ), this );
	}

	DebugReportCallback::~DebugReportCallback()
	{
	}

	void DebugReportCallback::report( GlDebugSource source
		, GlDebugType type
		, uint32_t id
		, GlDebugSeverity severity
		, int length
		, const char * const message )
	{
		auto flags = convert( type );
		flags |= convert( severity );

		if ( checkFlag( flags, m_createInfo.flags ) )
		{
			auto layer = convert( source );
			m_createInfo.pfnCallback( flags
				, VkDebugReportObjectTypeEXT::eUnknown
				, 0ull
				, 0u
				, id
				, layer
				, message
				, m_createInfo.pUserData );
		}
	}

	void DebugReportCallback::report( uint32_t id
		, GlDebugCategory category
		, GlDebugSeverity severity
		, int length
		, const char * const message )
	{
		auto flags = convert( severity );

		if ( checkFlag( flags, m_createInfo.flags ) )
		{
			auto layer = convert( category );
			m_createInfo.pfnCallback( flags
				, VkDebugReportObjectTypeEXT::eUnknown
				, 0ull
				, 0u
				, id
				, layer
				, message
				, m_createInfo.pUserData );
		}
	}
}
