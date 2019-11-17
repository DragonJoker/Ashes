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

namespace ashes::gl4
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
	bool glCheckError( std::string const & text );
}
