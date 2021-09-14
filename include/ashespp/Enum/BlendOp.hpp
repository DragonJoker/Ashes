/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BlendOp_HPP___
#define ___AshesPP_BlendOp_HPP___
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
	inline std::string getName( VkBlendOp value )
	{
		switch ( value )
		{
		case VK_BLEND_OP_ADD:
			return "add";
		case VK_BLEND_OP_SUBTRACT:
			return "subtract";
		case VK_BLEND_OP_REVERSE_SUBTRACT:
			return "rev_subtract";
		case VK_BLEND_OP_MIN:
			return "min";
		case VK_BLEND_OP_MAX:
			return "max";
#ifdef VK_EXT_blend_operation_advanced
		case VK_BLEND_OP_ZERO_EXT:
			return "zero";
		case VK_BLEND_OP_SRC_EXT:
			return "src";
		case VK_BLEND_OP_DST_EXT:
			return "dst";
		case VK_BLEND_OP_SRC_OVER_EXT:
			return "src_over";
		case VK_BLEND_OP_DST_OVER_EXT:
			return "dst_over";
		case VK_BLEND_OP_SRC_IN_EXT:
			return "src_in";
		case VK_BLEND_OP_DST_IN_EXT:
			return "dst_in";
		case VK_BLEND_OP_SRC_OUT_EXT:
			return "src_out";
		case VK_BLEND_OP_DST_OUT_EXT:
			return "dst_out";
		case VK_BLEND_OP_SRC_ATOP_EXT:
			return "src_atop";
		case VK_BLEND_OP_DST_ATOP_EXT:
			return "dst_atop";
		case VK_BLEND_OP_XOR_EXT:
			return "xor";
		case VK_BLEND_OP_MULTIPLY_EXT:
			return "multiply";
		case VK_BLEND_OP_SCREEN_EXT:
			return "screen";
		case VK_BLEND_OP_OVERLAY_EXT:
			return "overlay";
		case VK_BLEND_OP_DARKEN_EXT:
			return "darken";
		case VK_BLEND_OP_LIGHTEN_EXT:
			return "lighten";
		case VK_BLEND_OP_COLORDODGE_EXT:
			return "colordodge";
		case VK_BLEND_OP_COLORBURN_EXT:
			return "colorburn";
		case VK_BLEND_OP_HARDLIGHT_EXT:
			return "hardlight";
		case VK_BLEND_OP_SOFTLIGHT_EXT:
			return "softlight";
		case VK_BLEND_OP_DIFFERENCE_EXT:
			return "difference";
		case VK_BLEND_OP_EXCLUSION_EXT:
			return "exclusion";
		case VK_BLEND_OP_INVERT_EXT:
			return "invert";
		case VK_BLEND_OP_INVERT_RGB_EXT:
			return "invert_rgb";
		case VK_BLEND_OP_LINEARDODGE_EXT:
			return "lineardodge";
		case VK_BLEND_OP_LINEARBURN_EXT:
			return "linearburn";
		case VK_BLEND_OP_VIVIDLIGHT_EXT:
			return "vividlight";
		case VK_BLEND_OP_LINEARLIGHT_EXT:
			return "linearlight";
		case VK_BLEND_OP_PINLIGHT_EXT:
			return "pinlight";
		case VK_BLEND_OP_HARDMIX_EXT:
			return "hardmix";
		case VK_BLEND_OP_HSL_HUE_EXT:
			return "hsl_hue";
		case VK_BLEND_OP_HSL_SATURATION_EXT:
			return "hsl_saturation";
		case VK_BLEND_OP_HSL_COLOR_EXT:
			return "hsl_color";
		case VK_BLEND_OP_HSL_LUMINOSITY_EXT:
			return "hsl_luminosity";
		case VK_BLEND_OP_PLUS_EXT:
			return "plus";
		case VK_BLEND_OP_PLUS_CLAMPED_EXT:
			return "plus_clamped";
		case VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT:
			return "plus_clamped_alpha";
		case VK_BLEND_OP_PLUS_DARKER_EXT:
			return "plus_darker";
		case VK_BLEND_OP_MINUS_EXT:
			return "minus";
		case VK_BLEND_OP_MINUS_CLAMPED_EXT:
			return "minus_clamped";
		case VK_BLEND_OP_CONTRAST_EXT:
			return "contrast";
		case VK_BLEND_OP_INVERT_OVG_EXT:
			return "invert_ovg";
		case VK_BLEND_OP_RED_EXT:
			return "red";
		case VK_BLEND_OP_GREEN_EXT:
			return "green";
		case VK_BLEND_OP_BLUE_EXT:
			return "blue";
#endif
		default:
			assert( false && "Unsupported VkBlendOp." );
			return "Unsupported VkBlendOp";
		}
	}
}

#endif
