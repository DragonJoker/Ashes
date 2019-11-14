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

		inline VkPipelineInputAssemblyStateCreateInfo const & getInputAssemblyState()const
		{
			return m_inputAssemblyState;
		}

		inline bool hasVertexLayout()const
		{
			return !m_vertexAttributeDescriptions.empty()
				&& !m_vertexBindingDescriptions.empty();
		}

		inline VkPipelineVertexInputStateCreateInfo const & getVertexInputState()const
		{
			return m_vertexInputState;
		}

		inline float const * getBlendFactor()const
		{
			return m_colorBlendState.blendConstants;
		}

		inline uint32_t getSampleMask()const
		{
			return m_colorBlendState.attachmentCount == 0u
				? 0u
				: m_colorBlendState.pAttachments->colorWriteMask;
		}

		inline uint32_t getStencilRef()const
		{
			return bool( m_depthStencilState )
				? m_depthStencilState->front.reference
				: 0u;
		}

		inline VkPipelineLayout getLayout()const
		{
			return m_layout;
		}

		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}

		inline bool hasDynamicStateEnable( VkDynamicState state )const
		{
			return m_dynamicStates.end() != std::find( m_dynamicStates.begin()
				, m_dynamicStates.end()
				, state );
		}

	private:
		VkDevice m_device;
		VkPipelineLayout m_layout;
		//
		VkVertexInputBindingDescriptionArray m_vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray m_vertexAttributeDescriptions;
		VkPipelineVertexInputStateCreateInfo m_vertexInputState;
		//
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		//
		VkViewportArray m_stateViewports;
		VkScissorArray m_stateScissors;
		VkPipelineViewportStateCreateInfo m_viewportState;
		//
		VkPipelineRasterizationStateCreateInfo m_rasterizationState;
		//
		VkPipelineMultisampleStateCreateInfo m_multisampleState;
		//
		Optional< VkPipelineDepthStencilStateCreateInfo > m_depthStencilState;
		//
		VkPipelineColorBlendAttachmentStateArray m_colorBlendStateAttachments;
		VkPipelineColorBlendStateCreateInfo m_colorBlendState;
		//
		VkDynamicStateArray m_dynamicStates;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
		//
		size_t m_vertexInputStateHash;
	};
}

#endif
