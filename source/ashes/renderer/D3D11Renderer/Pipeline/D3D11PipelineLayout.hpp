/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

#include <unordered_set>

namespace ashes::d3d11
{
	class PipelineLayout
		: public NonCopyable
	{
		friend class Pipeline;

	public:
		PipelineLayout( VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );
		~PipelineLayout()noexcept;

		ShaderBindings const & getShaderBindings()const;
		uint32_t getDescriptorSetIndex( VkDescriptorSet set )const;

		VkDescriptorSetLayoutArray const & getDescriptorsLayouts()const
		{
			return m_setLayouts;
		}

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		void addPipeline( VkPipeline pipeline );
		void removePipeline( VkPipeline pipeline );

	private:
		VkDevice m_device;
		VkDescriptorSetLayoutArray m_setLayouts;
		VkPushConstantRangeArray m_pushConstantRanges;
		VkPipelineLayoutCreateInfo m_createInfo;
		ShaderBindings m_shaderBindings;
		std::unordered_set< VkPipeline > m_pipelines;
	};
}
