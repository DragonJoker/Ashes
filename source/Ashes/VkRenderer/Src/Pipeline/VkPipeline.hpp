/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Pipeline_HPP___
#define ___VkRenderer_Pipeline_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Pipeline/Pipeline.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public ashes::Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::GraphicsPipelineCreateInfo && createInfo );
		~Pipeline();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPipeline.
		*\~english
		*\brief
		*	VkPipeline implicit cast operator.
		*/
		inline operator VkPipeline const &()const
		{
			return m_pipeline;
		}

	private:
		Device const & m_device;
		PipelineLayout const & m_layout;
		std::vector< VkVertexInputAttributeDescription > m_vertexAttributes;
		std::vector< VkVertexInputBindingDescription > m_vertexBindings;
		VkPipelineVertexInputStateCreateInfo m_vertexInputState;
		RenderPass const & m_renderPass;
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
		VkPipelineRasterizationStateCreateInfo m_rasterisationState;
		std::vector< VkPipelineColorBlendAttachmentState > m_colourBlendStateAttachments;
		VkPipelineColorBlendStateCreateInfo m_colourBlendState;
		VkPipelineMultisampleStateCreateInfo m_multisampleState;
		std::vector< VkViewport > m_viewports;
		std::vector< VkRect2D > m_scissors;
		VkPipelineViewportStateCreateInfo m_viewportState;
		ashes::Optional< VkPipelineDepthStencilStateCreateInfo > m_depthStencilState;
		ashes::Optional< VkPipelineTessellationStateCreateInfo > m_tessellationState;
		std::vector< std::vector< VkSpecializationMapEntry > > m_specialisationEntries;
		std::map< VkShaderStageFlagBits, VkSpecializationInfo > m_specialisationInfos;
		std::vector< VkPipelineShaderStageCreateInfo > m_shaderStages;
		std::vector< VkDynamicStateEnable > m_dynamicStates;
		VkPipeline m_pipeline{ VK_NULL_HANDLE };
	};
}

#endif
