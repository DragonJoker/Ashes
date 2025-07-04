#include "Descriptor/D3D11DescriptorSet.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Core/D3D11Device.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Descriptor/D3D11DescriptorPool.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11ImageView.hpp"

#include "ashesd3d11_api.hpp"

#include <ashes/common/VkTypeTraits.hpp>

namespace ashes::D3D11_NAMESPACE
{
	DescriptorSet::DescriptorSet( VkDevice device
		, VkDescriptorPool pool
		, VkDescriptorSetLayout layout )
		: m_pool{ pool }
		, m_layout{ layout }
	{
		get( pool )->registerSet( get( this ) );

		for ( auto & binding : *get( layout ) )
		{
			LayoutBindingWrites bindingWrites
			{
				binding,
				{},
			};
			m_writes.try_emplace( binding.binding, bindingWrites );
		}

		for ( auto & [_, write] : m_writes )
		{
			switch ( write.binding.descriptorType )
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
				m_samplers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				m_combinedTextureSamplers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				m_sampledTextures.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				m_storageTextures.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
				m_uniformTexelBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				m_storageTexelBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				m_uniformBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				m_storageBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				m_dynamicUniformBuffers.push_back( &write );
				m_dynamicBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
				m_dynamicStorageBuffers.push_back( &write );
				m_dynamicBuffers.push_back( &write );
				break;
			case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
				m_inputAttachments.push_back( &write );
				break;
#if VK_EXT_inline_uniform_block
			case VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT:
				m_inlineUniforms.push_back( &write );
				break;
#endif
			default:
				reportUnsupported( device, "VkDescriptorType" );
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

	DescriptorSet::~DescriptorSet()noexcept
	{
		for ( auto const & inlineUbo : m_inlineUbos )
		{
			deallocate( inlineUbo->buffer, get( get( m_pool )->getDevice() )->getAllocationCallbacks() );
			deallocate( inlineUbo->memory, get( get( m_pool )->getDevice() )->getAllocationCallbacks() );
		}
	}

	void DescriptorSet::mergeWrites( LayoutBindingWrites & writes, VkWriteDescriptorSet const & write )
	{
		writes.writes.push_back( write );
		auto & myWrite = writes.writes.back();

		if ( myWrite.pImageInfo )
		{
			m_imagesInfos.emplace_back( myWrite.pImageInfo, myWrite.pImageInfo + myWrite.descriptorCount );
			myWrite.pImageInfo = m_imagesInfos.back().data();
		}

#if VK_EXT_inline_uniform_block

		if ( auto inlineUniform = tryGet< VkWriteDescriptorSetInlineUniformBlockEXT >( myWrite.pNext ) )
		{
			myWrite.descriptorCount /= inlineUniform->dataSize;
			auto device = get( m_pool )->getDevice();
			auto inlineUbo = createInlineUbo( device
				, *inlineUniform
				, Instance::getMemoryProperties()
				, ashes::D3D11_NAMESPACE::vkCreateBuffer
				, ashes::D3D11_NAMESPACE::vkGetBufferMemoryRequirements
				, ashes::D3D11_NAMESPACE::vkAllocateMemory
				, ashes::D3D11_NAMESPACE::vkBindBufferMemory
				, ashes::D3D11_NAMESPACE::vkMapMemory
				, ashes::D3D11_NAMESPACE::vkFlushMappedMemoryRanges
				, ashes::D3D11_NAMESPACE::vkUnmapMemory );
			myWrite.pBufferInfo = &inlineUbo->info;
			m_inlineUbos.emplace_back( std::move( inlineUbo ) );
		}

#endif

		if ( myWrite.pBufferInfo )
		{
			m_buffersInfos.emplace_back( myWrite.pBufferInfo, myWrite.pBufferInfo + myWrite.descriptorCount );
			myWrite.pBufferInfo = m_buffersInfos.back().data();
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

	void DescriptorSet::update()
	{
		reportUnsupported( get( m_pool )->getDevice(), "VkCopyDescriptorSet" );
	}
}
