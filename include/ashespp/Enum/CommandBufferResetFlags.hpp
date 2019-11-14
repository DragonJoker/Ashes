/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandBufferResetFlag_HPP___
#define ___AshesPP_CommandBufferResetFlag_HPP___
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
	inline std::string getName( VkCommandBufferResetFlagBits value )
	{
		switch ( value )
		{
		case VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT:
			return "release_resources";
		default:
			assert( false && "Unsupported VkCommandBufferResetFlagBits." );
			throw std::runtime_error{ "Unsupported VkCommandBufferResetFlagBits" };
		}

		return 0;
	}
}

#endif
