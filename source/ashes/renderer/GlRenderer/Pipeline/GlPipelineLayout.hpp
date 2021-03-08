/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/ShaderBindings.hpp>

#include <unordered_set>
#include <unordered_map>

namespace ashes::gl
{
	class PipelineLayout
		: public AutoIdIcdObject< PipelineLayout >
	{
		friend class Pipeline;

	public:
		PipelineLayout( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkPipelineLayoutCreateInfo createInfo );
		~PipelineLayout();

		ShaderBindings const & getShaderBindings()const;
		uint32_t getDescriptorSetIndex( VkDescriptorSet descriptorSet )const;
		ShaderBindings const & getDescriptorSetBindings( VkDescriptorSet descriptorSet
			, uint32_t descriptorSetIndex )const;

		VkPushConstantRangeArray const & getPushConstants()const
		{
			return m_pushConstantRanges;
		}

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
		mutable std::unordered_map< uint64_t, ShaderBindings > m_dsBindings;
		std::unordered_set< VkPipeline > m_pipelines;
	};
}
