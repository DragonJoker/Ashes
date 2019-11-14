/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DepthStencilState_HPP___
#define ___Ashes_DepthStencilState_HPP___
#pragma once

#include "ashes/renderer/RendererCommon/Helper/StencilOpState.hpp"

namespace ashes
{
	inline VkPipelineDepthStencilStateCreateInfo getDefaultDepthStencilState( void * pNext = nullptr )
	{
		return
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			pNext,
			0u,
			VK_TRUE,
			VK_TRUE,
			VK_COMPARE_OP_LESS,
			VK_FALSE,
			VK_FALSE,
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0u,
			},
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0u,
			},
			0.0f,
			1.0f,
		};
	};

	VkPipelineDepthStencilStateCreateInfo const & getDeactivatedDepthStencilState();

	inline bool operator==( VkPipelineDepthStencilStateCreateInfo const & lhs, VkPipelineDepthStencilStateCreateInfo const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.depthTestEnable == rhs.depthTestEnable
			&& lhs.depthWriteEnable == rhs.depthWriteEnable
			&& lhs.depthCompareOp == rhs.depthCompareOp
			&& lhs.depthBoundsTestEnable == rhs.depthBoundsTestEnable
			&& lhs.stencilTestEnable == rhs.stencilTestEnable
			&& lhs.front == rhs.front
			&& lhs.back == rhs.back
			&& lhs.minDepthBounds == rhs.minDepthBounds
			&& lhs.maxDepthBounds == rhs.maxDepthBounds;
	}

	inline bool operator!=( VkPipelineDepthStencilStateCreateInfo const & lhs, VkPipelineDepthStencilStateCreateInfo const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
