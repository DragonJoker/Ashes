/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ColorSpace_HPP___
#define ___AshesPP_ColorSpace_HPP___
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
	inline std::string getName( VkColorSpaceKHR value )
	{
		switch ( value )
		{
		case VK_COLORSPACE_SRGB_NONLINEAR_KHR:
			return "srgb_nonlinear";
#if defined( VK_EXT_swapchain_colorspace ) && VK_HEADER_VERSION > 39
		case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:
			return "display_p3_nonlinear";
		case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
			return "extended_srgb_linear";
		case VK_COLOR_SPACE_DCI_P3_LINEAR_EXT:
			return "dci_p3_linear";
		case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:
			return "dci_p3_nonlinear";
		case VK_COLOR_SPACE_BT709_LINEAR_EXT:
			return "bt709_linear";
		case VK_COLOR_SPACE_BT709_NONLINEAR_EXT:
			return "bt709_nonlinear";
		case VK_COLOR_SPACE_BT2020_LINEAR_EXT:
			return "bt2020_linear";
		case VK_COLOR_SPACE_HDR10_ST2084_EXT:
			return "hrd10_st2084";
		case VK_COLOR_SPACE_DOLBYVISION_EXT:
			return "dolby_vision";
		case VK_COLOR_SPACE_HDR10_HLG_EXT:
			return "hdr10_hlg";
		case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT:
			return "adobe_rgb_linear";
		case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:
			return "adobe_rgb_nonlinear";
		case VK_COLOR_SPACE_PASS_THROUGH_EXT:
			return "pass_through";
		case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT:
			return "extended_srgb_nonlinear";
#endif
		default:
			assert( false && "Unsupported VkColorSpaceKHR." );
			return "Unsupported VkColorSpaceKHR";
		}
	}
}

#endif
