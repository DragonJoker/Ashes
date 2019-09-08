/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CullModeFlag_HPP___
#define ___AshesPP_CullModeFlag_HPP___
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
	inline std::string getName( VkCullModeFlagBits value )
	{
		switch ( value )
		{
		case VK_CULL_MODE_FRONT_BIT:
			return "front";
		case VK_CULL_MODE_BACK_BIT:
			return "back";
		default:
			assert( false && "Unsupported VkCullModeFlagBits." );
			throw std::runtime_error{ "Unsupported VkCullModeFlagBits" };
		}

		return 0;
	}
}

#endif
