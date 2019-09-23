/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SwapChainCreateFlags_HPP___
#define ___AshesPP_SwapChainCreateFlags_HPP___
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
	inline std::string getName( VkSwapchainCreateFlagBitsKHR value )
	{
		switch ( value )
		{
		case VK_SWAPCHAIN_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT_KHR:
			return "split_instance_bind_regions";
		case VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR:
			return "protected";
#ifdef VK_EXT_swapchain_mutable_format
		case VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR:
			return "mutable_format";
#endif
		default:
			assert( false && "Unsupported VkSwapchainCreateFlagBitsKHR." );
			throw std::runtime_error{ "Unsupported VkSwapchainCreateFlagBitsKHR" };
		}

		return 0;
	}
}

#endif
