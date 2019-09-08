/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandPoolCreateFlags_HPP___
#define ___AshesPP_CommandPoolCreateFlags_HPP___
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
	inline std::string getName( VkCommandPoolCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_COMMAND_POOL_CREATE_TRANSIENT_BIT:
			return "transient";
		case VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT:
			return "reset_command_buffer";
		case VK_COMMAND_POOL_CREATE_PROTECTED_BIT:
			return "protected";
		default:
			assert( false && "Unsupported VkCommandPoolCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkCommandPoolCreateFlagBits" };
		}

		return 0;
	}
}

#endif
