#include "Descriptor/TestDescriptorSet.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Core/TestDevice.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Descriptor/TestDescriptorPool.hpp"
#include "Image/TestSampler.hpp"
#include "Image/TestImageView.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DescriptorSet::DescriptorSet( VkDevice device
		, VkDescriptorPool pool
		, VkDescriptorSetLayout layout )
		: m_layout{ layout }
	{
		get( pool )->registerSet( get( this ) );

		for ( auto & binding : *get( layout ) )
		{
			LayoutBindingWrites bindingWrites
			{
				binding,
				{},
			};
			m_writes.insert( { binding.binding, bindingWrites } );
		}

		for ( auto & write : m_writes )
		{
			switch ( write.second.binding.descriptorType )
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
				m_samplers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				m_combinedTextureSamplers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				m_sampledTextures.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				m_storageTextures.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
				m_texelBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				m_texelBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				m_uniformBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				m_storageBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				m_dynamicUniformBuffers.push_back( &write.second );
				m_dynamicBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				m_dynamicStorageBuffers.push_back( &write.second );
				m_dynamicBuffers.push_back( &write.second );
				break;
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
				break;
			default:
				break;
			}
		}

		std::sort( m_dynamicBuffers.begin()
			, m_dynamicBuffers.end()
			, []( LayoutBindingWrites const * lhs
				, LayoutBindingWrites const * rhs )
			{
				return lhs->binding.binding < rhs->binding.binding;
			} );
	}

	void DescriptorSet::mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write )
	{
		writes.writes.push_back( write );

		if ( write.pImageInfo )
		{
			m_imagesInfos.emplace_back( std::vector< VkDescriptorImageInfo >{ write.pImageInfo, write.pImageInfo + write.descriptorCount } );
			writes.writes.back().pImageInfo = m_imagesInfos.back().data();
		}

		if ( write.pBufferInfo )
		{
			m_buffersInfos.emplace_back( std::vector< VkDescriptorBufferInfo >{ write.pBufferInfo, write.pBufferInfo + write.descriptorCount } );
			writes.writes.back().pBufferInfo = m_buffersInfos.back().data();
		}
	}

	void DescriptorSet::update( VkWriteDescriptorSet const & write )
	{
		auto it = m_writes.find( write.dstBinding );
		assert( it != m_writes.end() );
		assert( it->second.binding.descriptorType == write.descriptorType );
		assert( it->second.binding.descriptorCount >= write.dstArrayElement + write.descriptorCount );
		assert( write.dstSet == get( this ) );
		mergeWrites( it->second, write );
	}

	void DescriptorSet::update( VkCopyDescriptorSet const & copy )
	{
		//m_writes[copy.dstBinding].dstBinding = copy.srcBinding;
		//m_writes[copy.dstBinding].dstArrayElement = copy.srcArrayElement;
	}
}
