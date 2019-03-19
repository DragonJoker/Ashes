/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Miscellaneous/SurfaceCapabilities.hpp>
#include <Ashes/Miscellaneous/SurfaceFormat.hpp>

namespace utils
{
	void getSurfaceInfos( ashes::WindowHandle const & handle
		, std::string & type
		, std::vector< ashes::SurfaceFormat > & formats
		, ashes::SurfaceCapabilities & capabilities );
}

