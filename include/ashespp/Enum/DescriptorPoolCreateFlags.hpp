/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorPoolCreateFlags_HPP___
#define ___AshesPP_DescriptorPoolCreateFlags_HPP___
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
	inline std::string getName( VkDescriptorPoolCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT:
			return "free_descriptor_set";
#ifdef VK_EXT_descriptor_indexing
		case VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT:
			return "update_after_bind";
#endif
		default:
			assert( false && "Unsupported VkDescriptorPoolCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkDescriptorPoolCreateFlagBits" };
		}

		return 0;
	}
}

#endif
