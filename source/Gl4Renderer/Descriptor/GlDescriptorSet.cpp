#include "Descriptor/GlDescriptorSet.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"

#include <algorithm>

namespace ashes::gl4
{
	void DescriptorSet::update( VkWriteDescriptorSet const & write )const
	{
		m_writes[write.dstBinding] = write;
	}

	void DescriptorSet::update( VkCopyDescriptorSet const & copy )const
	{
		m_writes[copy.dstBinding].dstBinding = copy.srcBinding;
		m_writes[copy.dstBinding].dstArrayElement = copy.srcArrayElement;
	}

	void DescriptorSet::update()const
	{
		m_combinedTextureSamplers.clear();
		m_samplers.clear();
		m_sampledTextures.clear();
		m_storageTextures.clear();
		m_uniformBuffers.clear();
		m_storageBuffers.clear();
		m_texelBuffers.clear();
		m_dynamicUniformBuffers.clear();
		m_dynamicStorageBuffers.clear();
		m_dynamicBuffers.clear();

		for ( auto & write : m_writes )
		{
			switch ( write.descriptorType )
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
				m_samplers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				m_combinedTextureSamplers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				m_sampledTextures.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				m_storageTextures.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
				m_texelBuffers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				m_texelBuffers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				m_uniformBuffers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				m_storageBuffers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				m_dynamicUniformBuffers.push_back( write );
				m_dynamicBuffers.push_back( write );
				break;
			case VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				m_dynamicStorageBuffers.push_back( write );
				m_dynamicBuffers.push_back( write );
				break;
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
				break;
			}
		}

		std::sort( m_dynamicBuffers.begin()
			, m_dynamicBuffers.end()
			, []( VkWriteDescriptorSet const & lhs
				, VkWriteDescriptorSet const & rhs )
		{
			return lhs.dstBinding < rhs.dstBinding;
		} );
	}
}
