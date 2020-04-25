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

		GraphicsPipelineCreateInfo( VkPipelineCreateFlags flags
			, PipelineShaderStageCreateInfoArray stages
			, PipelineVertexInputStateCreateInfo vertexInputState
			, PipelineInputAssemblyStateCreateInfo inputAssemblyState
			, Optional< PipelineTessellationStateCreateInfo > tessellationState
			, PipelineViewportStateCreateInfo viewportState
			, PipelineRasterizationStateCreateInfo rasterizationState
			, PipelineMultisampleStateCreateInfo multisampleState
			, Optional< PipelineDepthStencilStateCreateInfo > depthStencilState
			, PipelineColorBlendStateCreateInfo colorBlendState
			, Optional< PipelineDynamicStateCreateInfo > dynamicState
			, VkPipelineLayout layout
			, VkRenderPass renderPass
			, uint32_t subpass = 0u
			, VkPipeline basePipelineHandle = VK_NULL_HANDLE
			, int32_t basePipelineIndex = 0 )
			: stages{ std::move( stages ) }
			, vertexInputState{ std::move( vertexInputState ) }
			, inputAssemblyState{ std::move( inputAssemblyState ) }
			, tessellationState{ std::move( tessellationState ) }
			, viewportState{ std::move( viewportState ) }
			, rasterizationState{ std::move( rasterizationState ) }
			, multisampleState{ std::move( multisampleState ) }
			, depthStencilState{ std::move( depthStencilState ) }
			, colorBlendState{ std::move( colorBlendState ) }
			, dynamicState{ std::move( dynamicState ) }
			, vkStages{ makeVkArray< VkPipelineShaderStageCreateInfo >( stages ) }
			, vk
			{
				VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				nullptr,
				flags,
				uint32_t( this->vkStages.size() ),
				this->vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( this->vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( this->inputAssemblyState ),
				( bool( this->tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( this->tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( this->viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( this->rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( this->multisampleState ),
				( bool( this->depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( this->depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( this->colorBlendState ),
				( bool( this->dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( this->dynamicState.value() )
					: nullptr ),
				layout,
				renderPass,
				subpass,
				basePipelineHandle,
				basePipelineIndex,
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
				uint32_t( this->vkStages.size() ),
				this->vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( this->vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( this->inputAssemblyState ),
				( bool( this->tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( this->tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( this->viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( this->rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( this->multisampleState ),
				( bool( this->depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( this->depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( this->colorBlendState ),
				( bool( this->dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( this->dynamicState.value() )
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
				uint32_t( this->vkStages.size() ),
				this->vkStages.data(),
				&static_cast< VkPipelineVertexInputStateCreateInfo const & >( this->vertexInputState ),
				&static_cast< VkPipelineInputAssemblyStateCreateInfo const & >( this->inputAssemblyState ),
				( bool( this->tessellationState )
					? &static_cast< VkPipelineTessellationStateCreateInfo const & >( this->tessellationState.value() )
					: nullptr ),
				&static_cast< VkPipelineViewportStateCreateInfo const & >( this->viewportState ),
				&static_cast< VkPipelineRasterizationStateCreateInfo const & >( this->rasterizationState ),
				&static_cast< VkPipelineMultisampleStateCreateInfo const & >( this->multisampleState ),
				( bool( this->depthStencilState )
					? &static_cast< VkPipelineDepthStencilStateCreateInfo const & >( this->depthStencilState.value() )
					: nullptr ),
				&static_cast< VkPipelineColorBlendStateCreateInfo const & >( this->colorBlendState ),
				( bool( this->dynamicState )
					? &static_cast< VkPipelineDynamicStateCreateInfo const & >( this->dynamicState.value() )
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
