/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

#if ASHES_WIN32

#include <Ashes/Core/Surface.hpp>

namespace utils
{
	void getSurfaceInfos( ashes::WindowHandle const & handle
		, std::string & type
		, std::vector< ashes::SurfaceFormat > & formats
		, ashes::SurfaceCapabilities & capabilities );
}

#endif
