/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PipelineDepthStencilStateCreateFlags_HPP___
#define ___AshesPP_PipelineDepthStencilStateCreateFlags_HPP___
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
	inline std::string getName( VkPipelineDepthStencilStateCreateFlagBits value )
	{
		switch ( value )
		{
#if VK_EXT_rasterization_order_attachment_access
		case VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_EXT:
			return "rasterization_order_attachment_depth_access";
		case VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_EXT:
			return "rasterization_order_attachment_stencil_access";
#endif
		default:
			assert( false && "Unsupported VkPipelineDepthStencilStateCreateFlagBits." );
			return "Unsupported VkPipelineDepthStencilStateCreateFlagBits";
		}
	}
}

#endif
