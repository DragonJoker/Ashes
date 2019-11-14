/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

namespace ashes::gl4
{
	class PipelineLayout
	{
	public:
		PipelineLayout( VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );

		ShaderBindings const & getShaderBindings()const;
		uint32_t getDescriptorSetIndex( VkDescriptorSet set )const;

		inline VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const
		{
			return m_setLayouts;
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutArray m_setLayouts;
		VkPushConstantRangeArray m_pushConstantRanges;
		VkPipelineLayoutCreateInfo m_createInfo;
		ShaderBindings m_shaderBindings;
	};
}
