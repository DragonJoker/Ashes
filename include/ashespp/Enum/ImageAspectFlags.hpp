/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageAspectFlags_HPP___
#define ___AshesPP_ImageAspectFlags_HPP___
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
	inline std::string getName( VkImageAspectFlagBits value )
	{
		switch ( value )
		{
		case VK_IMAGE_ASPECT_COLOR_BIT:
			return "color";
		case VK_IMAGE_ASPECT_DEPTH_BIT:
			return "depth";
		case VK_IMAGE_ASPECT_STENCIL_BIT:
			return "stencil";
		case VK_IMAGE_ASPECT_METADATA_BIT:
			return "metadata";
		case VK_IMAGE_ASPECT_PLANE_0_BIT:
			return "plane_0";
		case VK_IMAGE_ASPECT_PLANE_1_BIT:
			return "plane_1";
		case VK_IMAGE_ASPECT_PLANE_2_BIT:
			return "plane_2";
		case VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT:
			return "memory_plane_0";
		case VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT:
			return "memory_plane_1";
		case VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT:
			return "memory_plane_2";
		case VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT:
			return "memory_plane_3";
		default:
			assert( false && "Unsupported VkImageAspectFlagBits." );
			throw std::runtime_error{ "Unsupported VkImageAspectFlagBits" };
		}

		return 0;
	}
}

#endif
