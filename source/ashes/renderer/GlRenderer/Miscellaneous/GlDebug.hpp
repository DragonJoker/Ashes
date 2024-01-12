/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#pragma warning( push )
#pragma warning( disable: 4365 )
#include <functional>
#pragma warning( pop )
#include <array>
#include <string>

#if !defined( CALLBACK )
#	if defined( _WIN32 )
#		define CALLBACK __stdcall
#	else
#		define CALLBACK
#	endif
#endif

namespace ashes::gl
{
	class ContextLock;

	struct DebugLabel
	{
		std::array< float, 4u > color;
		std::string labelName;
	};
	std::string getErrorName( uint32_t code, uint32_t category );
	bool glCheckError( ContextLock const & context
		, std::string const & text
		, bool log );
	bool glCheckError( ContextLock const & context
		, std::function< std::string() > const & stringifier
		, bool log );
	bool glCheckOutOfMemory( ContextLock const & context );
	void clearDebugFile();
	void logDebug( char const * const log );
	void logError( char const * const log );

	void pushDebugBlock( std::string const & name );
	void popDebugBlock();
}
