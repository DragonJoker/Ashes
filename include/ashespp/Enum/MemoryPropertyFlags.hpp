/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_MemoryPropertyFlags_HPP___
#define ___AshesPP_MemoryPropertyFlags_HPP___
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
	inline std::string getName( VkMemoryPropertyFlagBits value )
	{
		switch ( value )
		{
		case VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT:
			return "device_local";
		case VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT:
			return "host_visible";
		case VK_MEMORY_PROPERTY_HOST_COHERENT_BIT:
			return "host_coherent";
		case VK_MEMORY_PROPERTY_HOST_CACHED_BIT:
			return "host_cached";
		case VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT:
			return "lazily_allocated";
		case VK_MEMORY_PROPERTY_PROTECTED_BIT:
			return "protected";
		default:
			assert( false && "Unsupported VkMemoryPropertyFlagBits." );
			throw std::runtime_error{ "Unsupported VkMemoryPropertyFlagBits" };
		}

		return 0;
	}
}

#endif
