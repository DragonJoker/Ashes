#include "GlRendererPrerequisites.hpp"

#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlDebug.hpp"
#include "Miscellaneous/OpenGLDefines.hpp"

#include "ashesgl_api.hpp"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#if AshesGL_LogCalls
#	include <filesystem>
#endif

#ifdef _WIN32
#	include <gl/GL.h>
#endif

namespace ashes::gl
{
	namespace
	{
		static std::string const debugLogFile = "CallLogGL.log";
	}

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
		uint32_t errorCode = context->glGetError();

		if ( errorCode )
		{
			{
				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "OpenGL Error, on function: " << text;
				reportError( context->getInstance()
					, VkResult( errorCode )
					, stream.str()
					, getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) );
			}
#if AshesGL_LogCalls
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << "OpenGL Error, on function: " << text;
			stream << ", ID: 0x" << std::hex << errorCode << " (" << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) << ")";
			logStream( stream );
#endif
			errorCode = context->glGetError();
			result = false;
		}

		return result;
	}

#if AshesGL_LogCalls

	struct DebugLog
	{
		std::vector< std::string > blocks;

		static DebugLog & get()
		{
			thread_local DebugLog log;
			return log;
		}
	};

	void pushDebugBlock( std::string name )
	{
		DebugLog::get().blocks.push_back( name );
		logDebug( ( "BlockBegin: " + DebugLog::get().blocks.back() ).c_str() );
	}

	void popDebugBlock()
	{
		logDebug( ( "BlockEnd: " + DebugLog::get().blocks.back() ).c_str() );
		DebugLog::get().blocks.pop_back();
	}

	std::string getDebugPrefix()
	{
		return std::string( size_t( DebugLog::get().blocks.size() * 2u ), ' ' );
	}

	void clearDebugFile()
	{
		if ( std::filesystem::exists( debugLogFile ) )
		{
			std::filesystem::remove( debugLogFile );
		}
	}

	void logDebug( char const * const log )
	{
		std::ofstream file{ debugLogFile, std::ios::app };

		if ( file )
		{
			file << getDebugPrefix() << log << std::endl;
		}
	}

#else

	void clearDebugFile()
	{
	}

	void logDebug( char const * const log )
	{
	}

	void pushDebugBlock( std::string name )
	{
	}

	void popDebugBlock()
	{
	}

#endif

	void logStream( std::stringstream & stream )
	{
		logDebug( stream.str().c_str() );
	}

	//*************************************************************************************************
}
