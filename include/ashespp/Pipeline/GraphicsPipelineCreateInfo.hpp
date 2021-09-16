/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_GraphicsPipelineCreateInfo_HPP___
#define ___AshesPP_GraphicsPipelineCreateInfo_HPP___
#pragma once

#include "PipelineColorBlendStateCreateInfo.hpp"
#include "PipelineDepthStencilStateCreateInfo.hpp"
#include "PipelineDynamicStateCreateInfo.hpp"
#include "PipelineInputAssemblyStateCreateInfo.hpp"
#include "PipelineShaderStageCreateInfo.hpp"
#include "PipelineRasterizationStateCreateInfo.hpp"
#include "PipelineMultisampleStateCreateInfo.hpp"
#include "PipelineTessellationStateCreateInfo.hpp"
#include "PipelineVertexInputStateCreateInfo.hpp"
#include "PipelineViewportStateCreateInfo.hpp"

#include <map>

namespace ashes
{
	/**
	*\brief
	*	Holds the data used to create a graphic pipeline.
	*/
	struct GraphicsPipelineCreateInfo
	{
		GraphicsPipelineCreateInfo( GraphicsPipelineCreateInfo const & ) = delete;
		GraphicsPipelineCreateInfo & operator=( GraphicsPipelineCreateInfo const & ) = delete;

		GraphicsPipelineCreateInfo( VkPipelineCreateFlags pflags
			, PipelineShaderStageCreateInfoArray pstages
			, PipelineVertexInputStateCreateInfo pvertexInputState
			, PipelineInputAssemblyStateCreateInfo pinputAssemblyState
			, Optional< PipelineTessellationStateCreateInfo > ptessellationState
			, PipelineViewportStateCreateInfo pviewportState
			, PipelineRasterizationStateCreateInfo prasterizationState
			, PipelineMultisampleStateCreateInfo pmultisampleState
			, Optional< PipelineDepthStencilStateCreateInfo > pdepthStencilState
			, PipelineColorBlendStateCreateInfo pcolorBlendState
			, Optional< PipelineDynamicStateCreateInfo > pdynamicState
			, VkPipelineLayout playout
			, VkRenderPass prenderPass
			, uint32_t psubpass = {}
			, VkPipeline pbasePipelineHandle = {}
			, int32_t pbasePipelineIndex = {} )
			: stages{ std::move( pstages ) }
			, vertexInputState{ std::move( pvertexInputState ) }
			, inputAssemblyState{ std::move( pinputAssemblyState ) }
			, tessellationState{ std::move( ptessellationState ) }
			, viewportState{ std::move( pviewportState ) }
			, rasterizationState{ std::move( prasterizationState ) }
			, multisampleState{ std::move( pmultisampleState ) }
			, depthStencilState{ std::move( pdepthStencilState ) }
			, colorBlendState{ std::move( pcolorBlendState ) }
			, dynamicState{ std::move( pdynamicState ) }
			, vkStages{ makeVkArray< VkPipelineShaderStageCreateInfo >( stages ) }
			, vk
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				nullptr,
				pflags,
				uint32_t( vkStages.size() ),
				vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( inputAssemblyState ),
				( bool( tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( multisampleState ),
				( bool( depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( colorBlendState ),
				( bool( dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( dynamicState.value() )
					: nullptr ),
				playout,
				prenderPass,
				psubpass,
				pbasePipelineHandle,
				pbasePipelineIndex,
			}
		{
		}

		GraphicsPipelineCreateInfo( GraphicsPipelineCreateInfo && rhs )noexcept
			: stages{ std::move( rhs.stages ) }
			, vertexInputState{ std::move( rhs.vertexInputState ) }
			, inputAssemblyState{ std::move( rhs.inputAssemblyState ) }
			, tessellationState{ std::move( rhs.tessellationState ) }
			, viewportState{ std::move( rhs.viewportState ) }
			, rasterizationState{ std::move( rhs.rasterizationState ) }
			, multisampleState{ std::move( rhs.multisampleState ) }
			, depthStencilState{ std::move( rhs.depthStencilState ) }
			, colorBlendState{ std::move( rhs.colorBlendState ) }
			, dynamicState{ std::move( rhs.dynamicState ) }
			, vkStages{ makeVkArray< VkPipelineShaderStageCreateInfo >( stages ) }
			, vk
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vkStages.size() ),
				vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( inputAssemblyState ),
				( bool( tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( multisampleState ),
				( bool( depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( colorBlendState ),
				( bool( dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( dynamicState.value() )
					: nullptr ),
				rhs.vk.layout,
				rhs.vk.renderPass,
				rhs.vk.subpass,
				rhs.vk.basePipelineHandle,
				rhs.vk.basePipelineIndex,
			}
		{
		}

		GraphicsPipelineCreateInfo & operator=( GraphicsPipelineCreateInfo && rhs )noexcept
		{
			stages = std::move( rhs.stages );
			vertexInputState = std::move( rhs.vertexInputState );
			inputAssemblyState = std::move( rhs.inputAssemblyState );
			tessellationState = std::move( rhs.tessellationState );
			viewportState = std::move( rhs.viewportState );
			rasterizationState = std::move( rhs.rasterizationState );
			multisampleState = std::move( rhs.multisampleState );
			depthStencilState = std::move( rhs.depthStencilState );
			colorBlendState = std::move( rhs.colorBlendState );
			dynamicState = std::move( rhs.dynamicState );
			vkStages = makeVkArray< VkPipelineShaderStageCreateInfo >( stages );
			vk =
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				nullptr,
				rhs.vk.flags,
				uint32_t( vkStages.size() ),
				vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( inputAssemblyState ),
				( bool( tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( multisampleState ),
				( bool( depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( colorBlendState ),
				( bool( dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( dynamicState.value() )
					: nullptr ),
				rhs.vk.layout,
				rhs.vk.renderPass,
				rhs.vk.subpass,
				rhs.vk.basePipelineHandle,
				rhs.vk.basePipelineIndex,
			};

			return *this;
		}

		inline operator VkGraphicsPipelineCreateInfo const &()const
		{
			return vk;
		}

		PipelineShaderStageCreateInfoArray stages;
		PipelineVertexInputStateCreateInfo vertexInputState;
		PipelineInputAssemblyStateCreateInfo inputAssemblyState;
		Optional< PipelineTessellationStateCreateInfo > tessellationState;
		PipelineViewportStateCreateInfo viewportState;
		PipelineRasterizationStateCreateInfo rasterizationState;
		PipelineMultisampleStateCreateInfo multisampleState;
		Optional< PipelineDepthStencilStateCreateInfo > depthStencilState;
		PipelineColorBlendStateCreateInfo colorBlendState;
		Optional< PipelineDynamicStateCreateInfo > dynamicState;

	private:
		VkPipelineShaderStageCreateInfoArray vkStages;
		VkGraphicsPipelineCreateInfo vk;
	};
}

#endif
