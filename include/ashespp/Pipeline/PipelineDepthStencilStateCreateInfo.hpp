/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineDepthStencilStateCreateInfo_HPP___
#define ___Ashes_PipelineDepthStencilStateCreateInfo_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	struct PipelineDepthStencilStateCreateInfo
	{
		PipelineDepthStencilStateCreateInfo( PipelineDepthStencilStateCreateInfo const & ) = delete;
		PipelineDepthStencilStateCreateInfo & operator=( PipelineDepthStencilStateCreateInfo const & ) = delete;

		PipelineDepthStencilStateCreateInfo( VkPipelineDepthStencilStateCreateFlags flags = 0u
			, VkBool32 depthTestEnable = VK_TRUE
			, VkBool32 depthWriteEnable = VK_TRUE
			, VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS
			, VkBool32 depthBoundsTestEnable = VK_FALSE
			, VkBool32 stencilTestEnable = VK_FALSE
			, VkStencilOpState front =
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0x0u,
			}
			, VkStencilOpState back =
			{
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_STENCIL_OP_KEEP,
				VK_COMPARE_OP_ALWAYS,
				0xFFFFFFFFu,
				0xFFFFFFFFu,
				0x0u,
			}
			, float minDepthBounds = 0.0f
			, float maxDepthBounds = 1.0f )
			: vk
			{
				VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				nullptr,
				flags,
				depthTestEnable,
				depthWriteEnable,
				depthCompareOp,
				depthBoundsTestEnable,
				stencilTestEnable,
				front,
				back,
				minDepthBounds,
				maxDepthBounds,
			}
		{
		}
		
		PipelineDepthStencilStateCreateInfo( PipelineDepthStencilStateCreateInfo && rhs )
			: vk{ rhs.vk }
		{
		}
		
		PipelineDepthStencilStateCreateInfo & operator=( PipelineDepthStencilStateCreateInfo && rhs )
		{
			vk = rhs.vk;
			return *this;
		}

		inline operator VkPipelineDepthStencilStateCreateInfo const &()const
		{
			return vk;
		}

	private:
		VkPipelineDepthStencilStateCreateInfo vk;
	};
}

#endif
