/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_FenceCreateFlags_HPP___
#define ___AshesPP_FenceCreateFlags_HPP___
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
	inline std::string getName( VkFenceCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_FENCE_CREATE_SIGNALED_BIT:
			return "signaled";
		default:
			assert( false && "Unsupported VkFenceCreateFlagBits." );
			throw std::runtime_error{ "Unsupported VkFenceCreateFlagBits" };
		}

		return 0;
	}
}

#endif
