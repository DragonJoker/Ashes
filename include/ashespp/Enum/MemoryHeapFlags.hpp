/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MemoryHeapFlags_HPP___
#define ___AshesPP_MemoryHeapFlags_HPP___
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
	inline std::string getName( VkMemoryHeapFlagBits value )
	{
		switch ( value )
		{
		case VK_MEMORY_HEAP_DEVICE_LOCAL_BIT:
			return "device_local";
		case VK_MEMORY_HEAP_MULTI_INSTANCE_BIT:
			return "multi_instance";
		default:
			assert( false && "Unsupported VkMemoryHeapFlagBits." );
			return "Unsupported VkMemoryHeapFlagBits";
		}
	}
}

#endif
