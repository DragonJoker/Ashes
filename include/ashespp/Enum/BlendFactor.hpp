/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BlendFactor_HPP___
#define ___AshesPP_BlendFactor_HPP___
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
	inline std::string getName( VkBlendFactor value )
	{
		switch ( value )
		{
		case VK_BLEND_FACTOR_ZERO:
			return "zero";
		case VK_BLEND_FACTOR_ONE:
			return "one";
		case VK_BLEND_FACTOR_SRC_COLOR:
			return "src_colour";
		case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:
			return "inv_src_colour";
		case VK_BLEND_FACTOR_DST_COLOR:
			return "dst_colour";
		case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR:
			return "inv_dst_colour";
		case VK_BLEND_FACTOR_SRC_ALPHA:
			return "src_alpha";
		case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
			return "inv_src_alpha";
		case VK_BLEND_FACTOR_DST_ALPHA:
			return "dst_alpha";
		case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
			return "inv_dst_alpha";
		case VK_BLEND_FACTOR_CONSTANT_COLOR:
			return "constant_colour";
		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR:
			return "inv_constant_colour";
		case VK_BLEND_FACTOR_CONSTANT_ALPHA:
			return "constant_alpha";
		case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA:
			return "inv_constant_alpha";
		case VK_BLEND_FACTOR_SRC_ALPHA_SATURATE:
			return "src_alpha_saturate";
		case VK_BLEND_FACTOR_SRC1_COLOR:
			return "src1_colour";
		case VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR:
			return "inv_src1_colour";
		case VK_BLEND_FACTOR_SRC1_ALPHA:
			return "src1_alpha";
		case VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA:
			return "inv_src1_alpha";
		default:
			assert( false && "Unsupported VkBlendFactor." );
			throw std::runtime_error{ "Unsupported VkBlendFactor" };
		}

		return 0;
	}
}

#endif
