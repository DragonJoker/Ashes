/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageUsageFlags_HPP___
#define ___AshesPP_ImageUsageFlags_HPP___
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
	inline std::string getName( VkImageUsageFlagBits value )
	{
		switch ( value )
		{
		case VK_IMAGE_USAGE_TRANSFER_SRC_BIT:
			return "transfer_src";
		case VK_IMAGE_USAGE_TRANSFER_DST_BIT:
			return "transfer_dst";
		case VK_IMAGE_USAGE_SAMPLED_BIT:
			return "sampled";
		case VK_IMAGE_USAGE_STORAGE_BIT:
			return "storage";
		case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT:
			return "color_attachment";
		case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:
			return "depth_stencil_attachment";
		case VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT:
			return "transient_attachment";
		case VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT:
			return "input_attachment";
#ifdef VK_NV_shading_rate_image
		case VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV:
			return "shading_rate_image";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT:
			return "fragment_density_map";
#endif
		default:
			assert( false && "Unsupported VkImageUsageFlagBits." );
			return "Unsupported VkImageUsageFlagBits";
		}
	}
}

#endif
