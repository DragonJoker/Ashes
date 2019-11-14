/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	inline uint32_t makeShaderBindingKey( uint32_t set, uint32_t binding )
	{
		return ( set << 16u ) | ( binding << 0u );
	}

	class PipelineLayout
	{
	public:
		PipelineLayout( VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );

		ShaderBindingMap const & getShaderBindings()const;
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
		ShaderBindingMap m_shaderBindings;
	};
}
