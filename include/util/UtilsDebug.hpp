/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Utils_Debug_HPP___
#define ___Utils_Debug_HPP___
#pragma once

#include <ashespp/AshesPPPrerequisites.hpp>

namespace utils
{
#if VK_EXT_debug_utils
	VkDebugUtilsMessengerEXT setupDebugging( ashes::Instance const & instance
		, void * userData );
#elif VK_EXT_debug_report
	VkDebugReportCallbackEXT setupDebugging( ashes::Instance const & instance
		, void * userData );
#endif
}

#endif
