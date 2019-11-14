/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueueFlags_HPP___
#define ___AshesPP_QueueFlags_HPP___
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
	inline std::string getName( VkQueueFlagBits value )
	{
		switch ( value )
		{
		case VK_QUEUE_GRAPHICS_BIT:
			return "graphics";
		case VK_QUEUE_COMPUTE_BIT:
			return "compute";
		case VK_QUEUE_TRANSFER_BIT:
			return "transfer";
		case VK_QUEUE_SPARSE_BINDING_BIT:
			return "sparse_binding";
		case VK_QUEUE_PROTECTED_BIT:
			return "protected";
		default:
			assert( false && "Unsupported VkQueueFlagBits." );
			throw std::runtime_error{ "Unsupported VkQueueFlagBits" };
		}

		return 0;
	}
}

#endif
