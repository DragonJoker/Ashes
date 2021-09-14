/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MemoryAllocateFlags_HPP___
#define ___AshesPP_MemoryAllocateFlags_HPP___
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
	inline std::string getName( VkMemoryAllocateFlagBits value )
	{
		switch ( value )
		{
		case VK_MEMORY_ALLOCATE_DEVICE_MASK_BIT:
			return "device_mask";
#if defined( VK_API_VERSION_1_2 )
		case VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT:
			return "device_address";
		case VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT:
			return "address_capture_replay";
#elif defined( VK_KHR_buffer_device_address )
		case VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR:
			return "device_address";
		case VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR:
			return "address_capture_replay";
#endif
		default:
			assert( false && "Unsupported VkMemoryAllocateFlagBits." );
			return "Unsupported VkMemoryAllocateFlagBits";
		}
	}
}

#endif
