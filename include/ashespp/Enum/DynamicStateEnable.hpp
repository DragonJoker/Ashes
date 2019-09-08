/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DynamicStateEnable_HPP___
#define ___AshesPP_DynamicStateEnable_HPP___
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
	inline std::string getName( VkDynamicState value )
	{
		switch ( value )
		{
		case VK_DYNAMIC_STATE_VIEWPORT:
			return "viewport";
		case VK_DYNAMIC_STATE_SCISSOR:
			return "scissor";
		case VK_DYNAMIC_STATE_LINE_WIDTH:
			return "line_width";
		case VK_DYNAMIC_STATE_DEPTH_BIAS:
			return "depth_bias";
		case VK_DYNAMIC_STATE_BLEND_CONSTANTS:
			return "blend_constants";
		case VK_DYNAMIC_STATE_DEPTH_BOUNDS:
			return "depth_bounds";
		case VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK:
			return "stencil_compare_mask";
		case VK_DYNAMIC_STATE_STENCIL_WRITE_MASK:
			return "stencil_write_mask";
		case VK_DYNAMIC_STATE_STENCIL_REFERENCE:
			return "stencil_reference";
		case VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV:
			return "viewport_w_scaling";
		case VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT:
			return "discard_rectangle";
		case VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT:
			return "sample_locations";
		case VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV:
			return "viewport_shading_rate_palette";
		case VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV:
			return "viewport_coarse_sample_order";
		case VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV:
			return "exclusive_scissor";
		default:
			assert( false && "Unsupported VkDynamicState." );
			throw std::runtime_error{ "Unsupported VkDynamicState" };
		}

		return 0;
	}
}

#endif
