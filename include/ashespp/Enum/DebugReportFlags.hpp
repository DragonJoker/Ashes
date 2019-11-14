/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DebugReportFlags_HPP___
#define ___AshesPP_DebugReportFlags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkDebugReportFlagBitsEXT value )
	{
		switch ( value )
		{
		case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
			return "information";
		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
			return "warning";
		case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
			return "performance_warning";
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			return "error";
		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
			return "debug";
		default:
			assert( false && "Unsupported VkDebugReportFlagBitsEXT." );
			throw std::runtime_error{ "Unsupported VkDebugReportFlagBitsEXT" };
		}

		return 0;
	}
}

#endif
