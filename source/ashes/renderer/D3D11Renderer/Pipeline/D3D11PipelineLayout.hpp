/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

namespace ashes::d3d11
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
	};
}
