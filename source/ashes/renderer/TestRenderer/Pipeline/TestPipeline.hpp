/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Pipeline_HPP___
#define ___TestRenderer_Pipeline_HPP___
#pragma once

#include "renderer/TestRenderer/Shader/TestShaderModule.hpp"

namespace ashes::test
{
	class Pipeline
	{
	public:
		Pipeline( VkDevice device
			, VkGraphicsPipelineCreateInfo createInfo );
		Pipeline( VkDevice device
			, VkComputePipelineCreateInfo createInfo );
		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const;

		VkPipelineInputAssemblyStateCreateInfo const & getInputAssemblyState()const noexcept
		{
			return m_inputAssemblyState;
		}

		bool hasVertexLayout()const noexcept
		{
			return !m_vertexAttributeDescriptions.empty()
				&& !m_vertexBindingDescriptions.empty();
		}

		VkPipelineVertexInputStateCreateInfo const & getVertexInputState()const noexcept
		{
			return m_vertexInputState;
		}

		float const * getBlendFactor()const noexcept
		{
			return m_colorBlendState.blendConstants;
		}

		uint32_t getSampleMask()const noexcept
		{
			return m_colorBlendState.attachmentCount == 0u
				? 0u
				: m_colorBlendState.pAttachments->colorWriteMask;
		}

		uint32_t getStencilRef()const noexcept
		{
			return bool( m_depthStencilState )
				? m_depthStencilState->front.reference
				: 0u;
		}

		VkPipelineLayout getLayout()const noexcept
		{
			return m_layout;
		}

		size_t getVertexInputStateHash()const noexcept
		{
			return m_vertexInputStateHash;
		}

		bool hasDynamicStateEnable( VkDynamicState state )const noexcept
		{
			return m_dynamicStates.end() != std::find( m_dynamicStates.begin()
				, m_dynamicStates.end()
				, state );
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device{};
		VkPipelineLayout m_layout{};
		//
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		VkPipelineVertexInputStateCreateInfo m_vertexInputState{};
		//
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState{};
		//
		VkViewportArray m_stateViewports;
		VkScissorArray m_stateScissors;
		VkPipelineViewportStateCreateInfo m_viewportState{};
		//
		VkPipelineRasterizationStateCreateInfo m_rasterizationState{};
		//
		VkPipelineMultisampleStateCreateInfo m_multisampleState{};
		//
		Optional< VkPipelineDepthStencilStateCreateInfo > m_depthStencilState{ ashes::nullopt };
		//
		VkPipelineColorBlendAttachmentStateArray m_colorBlendStateAttachments;
		VkPipelineColorBlendStateCreateInfo m_colorBlendState{};
		//
		VkDynamicStateArray m_dynamicStates;
		VkPipelineDynamicStateCreateInfo m_dynamicState{};
		//
		size_t m_vertexInputStateHash{};
	};
}

#endif
