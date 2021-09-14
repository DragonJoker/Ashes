/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandBufferUsageFlags_HPP___
#define ___AshesPP_CommandBufferUsageFlags_HPP___
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
	inline std::string getName( VkCommandBufferUsageFlagBits value )
	{
		switch ( value )
		{
		case VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT:
			return "one_time_submit";
		case VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT:
			return "render_pass_continue";
		case VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT:
			return "simultaneous_use";
		default:
			assert( false && "Unsupported VkCommandBufferUsageFlagBits." );
			return "Unsupported VkCommandBufferUsageFlagBits";
		}
	}
}

#endif
