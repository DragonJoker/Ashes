/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageLayout_HPP___
#define ___AshesPP_ImageLayout_HPP___
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
	inline std::string getName( VkImageLayout value )
	{
		switch ( value )
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			return "undefined";
		case VK_IMAGE_LAYOUT_GENERAL:
			return "general";
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			return "color_attachment_optimal";
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			return "depth_stencil_attachment_optimal";
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			return "depth_stencil_read_only_optimal";
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			return "shader_read_only_optimal";
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			return "transfer_src_optimal";
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			return "transfer_dst_optimal";
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			return "preinitialized";
		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
			return "depth_read_only_stencil_attachment_optimal";
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
			return "depth_attachment_stencil_read_only_optimal";
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			return "present_src";
#ifdef VK_KHR_shared_presentable_image
		case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
			return "shared_present";
#endif
#ifdef VK_NV_shading_rate_image
		case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
			return "shading_rate_optimal";
#endif
#ifdef VK_EXT_fragment_density_map
		case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
			return "fragment_density_map_optimal";
#endif
		default:
			assert( false && "Unsupported VkImageLayout." );
			throw std::runtime_error{ "Unsupported VkImageLayout" };
		}

		return 0;
	}
}

#endif
