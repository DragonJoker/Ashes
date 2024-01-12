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
#include <sys/stat.h>

#ifdef _WIN32
#	include <gl/GL.h>
#endif

namespace ashes::gl
{
	namespace
	{
		std::string getDebugLogFile()
		{
			static std::string const debugLogBaseName = "CallLogGL.log";
			return  debugLogBaseName + toString( std::this_thread::get_id() ) + ".log";
		}
	}

	std::string getErrorName( uint32_t code, uint32_t category )
	{
		if ( category == GL_DEBUG_CATEGORY_API_ERROR_AMD
			|| category == GL_DEBUG_TYPE_ERROR )
		{
			return getName( GlError( code ) );
		}
		else
		{
			return std::string{};
		}
	}

	bool glCheckError( ContextLock const & context
		, std::string const & text
		, bool log )
	{
		return glCheckError( context
			, [&text]()
			{
				return text;
			}
			, log );
	}

	bool glCheckOutOfMemory( ContextLock const & context )
	{
		bool result = true;

		if ( auto errorCode = context->glGetError();
			errorCode == GL_ERROR_OUT_OF_MEMORY )
		{
			context->setOutOfMemory();
			result = false;
		}

		return result;
	}

	bool glCheckError( ContextLock const & context
		, std::function< std::string() > const & stringifier
		, bool log )
	{
		bool result = true;

		if ( auto errorCode = context->glGetError();
			errorCode != GL_SUCCESS )
		{
			if ( errorCode == GL_ERROR_OUT_OF_MEMORY )
			{
				context->setOutOfMemory();
			}

			if ( log )
			{
				auto text = stringifier();
				{
					std::stringstream stream;
					stream.imbue( std::locale{ "C" } );
					stream << "OpenGL Error, on function: " << text;
					reportError( context->getInstance()
						, VkResult( errorCode )
						, stream.str()
						, getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) );
				}

				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "OpenGL Error, on function: " << text;
				stream << ", ID: 0x" << std::hex << errorCode << " (" << getErrorName( errorCode, GL_DEBUG_TYPE_ERROR ) << ")";
				logError( stream.str().c_str() );
			}

			context->glGetError();
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

	void logDebug( char const * const log )
	{
		std::ofstream file{ getDebugLogFile(), std::ios::app };

		if ( file )
		{
			file << getDebugPrefix() << log << std::endl;
		}

#if defined( _WIN32 )
		if ( ::IsDebuggerPresent() )
		{
			::OutputDebugStringA( log );
			::OutputDebugStringA( "\n" );
		}
#endif
	}

#else

	void logDebug( char const * const )
	{
	}

	void pushDebugBlock( std::string const & )
	{
	}

	void popDebugBlock()
	{
	}

#endif

	void clearDebugFile()
	{
		struct stat buffer;

		if ( stat( getDebugLogFile().c_str(), &buffer ) == 0 )
		{
			remove( getDebugLogFile().c_str() );
		}
	}

	void logError( char const * const log )
	{
#if !defined( NDEBUG )

		if ( std::ofstream file{ getDebugLogFile(), std::ios::app };
			file )
		{
			file << log << std::endl;
		}

#if defined( _WIN32 )
		if ( ::IsDebuggerPresent() )
		{
			::OutputDebugStringA( log );
			::OutputDebugStringA( "\n" );
		}
#endif
#endif
	}

	void logStream( std::stringstream const & stream )
	{
		logDebug( stream.str().c_str() );
	}

	//*************************************************************************************************
}
