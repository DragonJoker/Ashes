/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

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
		, std::string const & text );
	void clearDebugFile();
	void logDebug( char const * const log );

	void pushDebugBlock( std::string name );
	void popDebugBlock();
}
