/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageViewType_HPP___
#define ___AshesPP_ImageViewType_HPP___
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
	inline std::string getName( VkImageViewType value )
	{
		switch ( value )
		{
		case VK_IMAGE_VIEW_TYPE_1D:
			return "1d";
		case VK_IMAGE_VIEW_TYPE_2D:
			return "2d";
		case VK_IMAGE_VIEW_TYPE_3D:
			return "3d";
		case VK_IMAGE_VIEW_TYPE_CUBE:
			return "cube";
		case VK_IMAGE_VIEW_TYPE_1D_ARRAY:
			return "1d_array";
		case VK_IMAGE_VIEW_TYPE_2D_ARRAY:
			return "2d_array";
		case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:
			return "cube_array";
		default:
			assert( false && "Unsupported VkImageViewType." );
			return "Unsupported VkImageViewType";
		}
	}
}

#endif
