/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PresentMode_HPP___
#define ___AshesPP_PresentMode_HPP___
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
	inline std::string getName( VkPresentModeKHR value )
	{
		switch ( value )
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR:
			return "immediate";
		case VK_PRESENT_MODE_MAILBOX_KHR:
			return "mailbox";
		case VK_PRESENT_MODE_FIFO_KHR:
			return "fifo";
		case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
			return "fifo_relaxed";
		case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
			return "shared_demand_refresh";
		case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
			return "shared_continuous_refresh";
		default:
			assert( false && "Unsupported VkPresentModeKHR." );
			return "Unsupported VkPresentModeKHR";
		}
	}
}

#endif
