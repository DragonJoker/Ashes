/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DependencyFlags_HPP___
#define ___AshesPP_DependencyFlags_HPP___
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
	inline std::string getName( VkDependencyFlagBits value )
	{
		switch ( value )
		{
		case VK_DEPENDENCY_BY_REGION_BIT:
			return "by_region";
#ifdef VK_KHR_device_group
		case VK_DEPENDENCY_DEVICE_GROUP_BIT:
			return "device_group";
		case VK_DEPENDENCY_VIEW_LOCAL_BIT:
			return "view_local";
#endif
		default:
			assert( false && "Unsupported VkDependencyFlagBits." );
			throw std::runtime_error{ "Unsupported VkDependencyFlagBits" };
		}

		return 0;
	}
}

#endif
