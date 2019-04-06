/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Utils_Debug_HPP___
#define ___Utils_Debug_HPP___
#pragma once

#include <AshesPP/AshesPPPrerequisites.hpp>

namespace utils
{
	VkDebugReportCallbackEXT setupDebugging( ashes::Instance const & instance
		, void * userData );
}

#endif
