#include "GlRendererPrerequisites.hpp"

#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlDebug.hpp"
#include "Miscellaneous/OpenGLDefines.hpp"

#include "ashesgl4_api.hpp"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#ifdef _WIN32
#	include <gl/GL.h>
#endif

namespace ashes::gl4
{
	std::string getErrorName( uint32_t code, uint32_t category )
	{
		static uint32_t constexpr InvalidEnum = 0x0500;
		static uint32_t constexpr InvalidValue = 0x0501;
		static uint32_t constexpr InvalidOperation = 0x0502;
		static uint32_t constexpr StackOverflow = 0x0503;
		static uint32_t constexpr StackUnderflow = 0x0504;
		static uint32_t constexpr OutOfMemory = 0x0505;
		static uint32_t constexpr InvalidFramebufferOperation = 0x0506;

		static std::map< uint32_t, std::string > const errors
		{
			{ InvalidEnum, "Invalid Enum" },
			{ InvalidValue, "Invalid Value" },
			{ InvalidOperation, "Invalid Operation" },
			{ StackOverflow, "Stack Overflow" },
			{ StackUnderflow, "Stack Underflow" },
			{ OutOfMemory, "Out of memory" },
			{ InvalidFramebufferOperation, "Invalid frame buffer operation" },
		};

		if ( category == GL_DEBUG_CATEGORY_API_ERROR_AMD
			|| category == GL_DEBUG_TYPE_ERROR )
		{
			auto it = errors.find( code );

			if ( it == errors.end() )
			{
				return "UNKNOWN_ERROR";
			}

			return it->second;
		}
		else
		{
			return std::string{};
		}
	}

	bool glCheckError( ContextLock const & context
		, std::string const & text )
	{
		bool result = true;
		uint32_t errorCode = context->m_glGetError();

		if ( errorCode )
		{
			auto instance = context->getInstance();
#if VK_EXT_debug_utils
			{
				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "OpenGL Error, on function: " << text;
				VkDebugUtilsObjectNameInfoEXT object
				{
					VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
					nullptr,
					VK_OBJECT_TYPE_INSTANCE,
					uint64_t( instance ),
					"OpenGL Instance",
				};
				get( instance )->submitDebugUtilsMessenger( VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
					, VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
					, {
						VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
						nullptr,
						0u,
						getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ).c_str(),
						int32_t( errorCode ),
						stream.str().c_str(),
						0u,
						nullptr,
						0u,
						nullptr,
						1u,
						&object,
					} );
			}
#endif
#if VK_EXT_debug_report
			{
				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "OpenGL Error, on function: " << text;
				stream << ", " << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR );
				context->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
					, VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT
					, uint64_t( instance )
					, 0u
					, errorCode
					, "OpenGL"
					, stream.str().c_str() );
			}
#endif
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "OpenGL Error, on function: " << text;
			stream << ", ID: 0x" << std::hex << errorCode << " (" << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) << ")";
			logStream( stream );
			errorCode = context->m_glGetError();
			result = false;
		}

		return result;
	}

	bool glCheckError( std::string const & text )
	{
		static PFN_glGetError getError = []()
		{
#ifdef _WIN32
			return glGetError;
#else
			PFN_glGetError result;
			getFunction( "glGetError", result );
			return result;
			
#endif
		}();
		bool result = true;
		uint32_t errorCode = getError();

		if ( errorCode )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "OpenGL Error, on function: " << text;
			stream << ", ID: 0x" << std::hex << errorCode << " (" << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) << ")";
			logStream( stream );
			errorCode = getError();
			result = false;
		}

		return result;
	}

	void logStream( std::stringstream & stream )
	{
#if GL_LOG_CALLS
		std::ofstream file{ "CallLogGL4.log", std::ios::app };

		if ( file )
		{
			file << stream.str() << std::endl;
		}
#endif
	}

	//*************************************************************************************************
}
