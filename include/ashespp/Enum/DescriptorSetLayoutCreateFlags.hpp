/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSetLayoutCreateFlags_HPP___
#define ___AshesPP_DescriptorSetLayoutCreateFlags_HPP___
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
	inline std::string getName( VkDescriptorSetLayoutCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR:
			return "create_push_descriptor";
#if defined( VK_API_VERSION_1_2 )
		case VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT:
			return "update_after_bind_pool";
#elif defined( VK_EXT_descriptor_indexing )
		case VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT_EXT:
			return "update_after_bind_pool";
#endif
		default:
			assert( false && "Unsupported VkDescriptorSetLayoutCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkDescriptorSetLayoutCreateFlagBits" };
		}

		return 0;
	}
}

#endif
