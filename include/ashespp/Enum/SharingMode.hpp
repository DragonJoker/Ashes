/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SharingMode_HPP___
#define ___AshesPP_SharingMode_HPP___
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
	inline std::string getName( VkSharingMode value )
	{
		switch ( value )
		{
		case VK_SHARING_MODE_EXCLUSIVE:
			return "exclusive";
		case VK_SHARING_MODE_CONCURRENT:
			return "concurrent";
		default:
			assert( false && "Unsupported VkSharingMode." );
			return "Unsupported VkSharingMode";
		}
	}
}

#endif
