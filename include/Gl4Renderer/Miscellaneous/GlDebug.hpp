/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

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
	std::string getErrorName( uint32_t code, uint32_t category );
	bool glCheckError( std::string const & text );
}
