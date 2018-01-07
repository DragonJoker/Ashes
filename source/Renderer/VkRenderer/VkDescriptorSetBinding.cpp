/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "VkDescriptorSetBinding.hpp"

#include "VkBuffer.hpp"
#include "VkDescriptorSetLayoutBinding.hpp"
#include "VkDescriptorSet.hpp"
#include "VkSampler.hpp"
#include "VkTexture.hpp"
#include "VkTextureView.hpp"
#include "VkUniformBuffer.hpp"

namespace vk_renderer
{
	//************************************************************************************************

	CombinedTextureSamplerBinding::CombinedTextureSamplerBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, DescriptorSet const & descriptorSet
		, Texture const & texture
		, Sampler const & sampler )
		: renderer::CombinedTextureSamplerBinding{ layoutBinding
			, texture
			, sampler }
		, m_view{ texture.getView() }
		, m_sampler{ sampler }
		, m_info
		{
			m_sampler,                                      // sampler
			m_view,                                         // imageView
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL        // imageLayout
		}
		, m_write
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,         // sType
			nullptr,                                        // pNext
			descriptorSet,                                  // dstSet
			layoutBinding.getBindingPoint(),                // dstBinding
			0u,                                             // dstArrayElement
			1u,                                             // descriptorCount
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,      // descriptorType
			&m_info,                                        // pImageInfo
			nullptr,                                        // pBufferInfo
			nullptr                                         // pTexelBufferView
		}
	{
	}

	//************************************************************************************************

	SampledTextureBinding::SampledTextureBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, DescriptorSet const & descriptorSet
		, Texture const & texture )
		: renderer::SampledTextureBinding{ layoutBinding, texture }
		, m_view{ texture.getView() }
		, m_info
		{
			VK_NULL_HANDLE,                                 // sampler
			m_view,                                         // imageView
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL        // imageLayout
		}
		, m_write
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,         // sType
			nullptr,                                        // pNext
			descriptorSet,                                  // dstSet
			layoutBinding.getBindingPoint(),                // dstBinding
			0u,                                             // dstArrayElement
			1u,                                             // descriptorCount
			VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,               // descriptorType
			&m_info,                                        // pImageInfo
			nullptr,                                        // pBufferInfo
			nullptr                                         // pTexelBufferView
		}
	{
	}

	//************************************************************************************************

	UniformBufferBinding::UniformBufferBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, DescriptorSet const & descriptorSet
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
		: renderer::UniformBufferBinding{ layoutBinding, uniformBuffer, offset }
		, m_uniformBuffer{ static_cast< Buffer const & >( uniformBuffer.getBuffer() ) }
		, m_info
		{
			m_uniformBuffer,                                // buffer
			offset,                                         // offset
			uniformBuffer.getSize()                         // range
		}
		, m_write
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,         // sType
			nullptr,                                        // pNext
			descriptorSet,                                  // dstSet
			layoutBinding.getBindingPoint(),                // dstBinding
			0u,                                             // dstArrayElement
			1u,                                             // descriptorCount
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,              // descriptorType
			nullptr,                                        // pImageInfo
			&m_info,                                        // pBufferInfo
			nullptr                                         // pTexelBufferView
		}
	{
	}

	//************************************************************************************************
}
