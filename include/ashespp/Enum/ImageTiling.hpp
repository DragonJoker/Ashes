/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageTiling_HPP___
#define ___AshesPP_ImageTiling_HPP___
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
	inline std::string getName( VkImageTiling value )
	{
		switch ( value )
		{
		case VK_IMAGE_TILING_OPTIMAL:
			return "optimal";
		case VK_IMAGE_TILING_LINEAR:
			return "linear";
#ifdef VK_EXT_image_drm_format_modifier
		case VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT:
			return "drm_format_modifier";
#endif
		default:
			assert( false && "Unsupported VkImageTiling." );
			throw std::runtime_error{ "Unsupported VkImageTiling" };
		}

		return 0;
	}
}

#endif
