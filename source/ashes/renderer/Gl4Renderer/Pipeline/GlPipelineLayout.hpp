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
		: public AutoIdIcdObject< PipelineLayout >
	{
	public:
		PipelineLayout( VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );

		ShaderBindings const & getShaderBindings()const;
		uint32_t getDescriptorSetIndex( VkDescriptorSet descriptorSet )const;
		ShaderBindings const & getDecriptorSetBindings( VkDescriptorSet descriptorSet
			, uint32_t descriptorSetIndex )const;

		inline VkPushConstantRangeArray const & getPushConstants()const
		{
			return m_pushConstantRanges;
		}

		inline VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const
		{
			return m_setLayouts;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutArray m_setLayouts;
		VkPushConstantRangeArray m_pushConstantRanges;
		VkPipelineLayoutCreateInfo m_createInfo;
		ShaderBindings m_shaderBindings;
		mutable std::unordered_map< uint64_t, ShaderBindings > m_dsBindings;
	};
}
