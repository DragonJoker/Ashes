/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RenderPassCreateFlags_HPP___
#define ___AshesPP_RenderPassCreateFlags_HPP___
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
	inline std::string getName( VkRenderPassCreateFlags value )
	{
		switch ( value )
		{
#if VK_QCOM_render_pass_transform
		case VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM:
			return "transform";
#endif
		default:
			assert( false && "Unsupported VkRenderPassCreateFlagBits." );
			return "Unsupported VkRenderPassCreateFlagBits";
		}
	}
}

#endif
