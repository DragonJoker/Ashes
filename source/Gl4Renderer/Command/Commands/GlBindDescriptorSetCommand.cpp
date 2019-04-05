/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindDescriptorSetCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		VkImageView getView( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return write.pImageInfo[index].imageView;
		}

		VkSampler getSampler( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return write.pImageInfo[index].sampler;
		}

		VkBuffer getBuffer( VkWriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.descriptorCount );
			return write.pBufferInfo[index].buffer;
		}

		void bindCombinedSampler( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				auto sampler = getSampler( write, i );
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindTexture
					, convertViewType( get( view )->getType() )
					, get( view )->getInternal() );
				glLogCall( context
					, glBindSampler
					, bindingIndex
					, get( sampler )->getInternal() );
			}
		}

		void bindSampler( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto sampler = getSampler( write, i );
				glLogCall( context
					, glBindSampler
					, bindingIndex
					, get( sampler )->getInternal() );
			}
		}

		void bindSampledTexture( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindTexture
					, convertViewType( get( view )->getType() )
					, get( view )->getInternal() );
			}
		}

		void bindStorageTexture( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto view = getView( write, i );
				auto & range = get( view )->getSubresourceRange();
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindImageTexture
					, bindingIndex
					, get( view )->getInternal()
					, range.baseMipLevel
					, range.layerCount
					, range.baseArrayLayer
					, GL_ACCESS_TYPE_READ_WRITE
					, getInternalFormat( get( view )->getFormat() ) );
			}
		}

		void bindUniformBuffer( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_UNIFORM
					, bindingIndex
					, get( buffer )->getInternal()
					, GLintptr( write.pBufferInfo[i].offset )
					, GLsizeiptr( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) );
			}
		}

		void bindStorageBuffer( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, bindingIndex
					, get( buffer )->getInternal()
					, GLintptr( write.pBufferInfo[i].offset )
					, GLsizeiptr( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) );
			}
		}

		void bindTexelBuffer( ContextLock const & context
			, VkWriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindTexture
					, GL_BUFFER_TARGET_TEXTURE
					, get( write.pTexelBufferView[i] )->getImage() );
			}
		}

		void bindDynamicUniformBuffer( ContextLock const & context
			, VkWriteDescriptorSet const & write, uint32_t offset )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_UNIFORM
					, bindingIndex
					, get( buffer )->getInternal()
					, GLintptr( write.pBufferInfo[i].offset + offset )
					, GLsizeiptr( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) );
			}
		}

		void bindDynamicStorageBuffer( ContextLock const & context
			, VkWriteDescriptorSet const & write, uint32_t offset )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, bindingIndex
					, get( buffer )->getInternal()
					, GLintptr( write.pBufferInfo[i].offset + offset )
					, GLsizeiptr( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) );
			}
		}

		void bindDynamicBuffers( ContextLock const & context
			, VkWriteDescriptorSetArray const & writes
			, UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & write = writes[i];

				switch ( write.descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					bindDynamicUniformBuffer( context, write, offsets[i] );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					bindDynamicStorageBuffer( context, write, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}

		void bindCombinedSampler( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindCombinedSampler( context, write );
			}
		}

		void bindSampler( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindSampler( context, write );
			}
		}

		void bindSampledTexture( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindSampledTexture( context, write );
			}
		}

		void bindStorageTexture( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageTexture( context, write );
			}
		}

		void bindUniformBuffer( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindUniformBuffer( context, write );
			}
		}

		void bindStorageBuffer( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageBuffer( context, write );
			}
		}

		void bindTexelBuffer( ContextLock const & context
			, LayoutBindingWrites const * writes )
		{
			for ( auto & write : writes->writes )
			{
				bindTexelBuffer( context, write );
			}
		}

		void bindDynamicUniformBuffer( ContextLock const & context
			, LayoutBindingWrites const * writes
			, uint32_t offset )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicUniformBuffer( context, write, offset );
			}
		}

		void bindDynamicStorageBuffer( ContextLock const & context
			, LayoutBindingWrites const * writes
			, uint32_t offset )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicStorageBuffer( context, write, offset );
			}
		}

		void bindDynamicBuffers( ContextLock const & context
			, LayoutBindingWritesArray const & writes
			, UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & write = writes[i];

				switch ( write->descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					bindDynamicUniformBuffer( context, write, offsets[i] );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					bindDynamicStorageBuffer( context, write, offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}
	}

	BindDescriptorSetCommand::BindDescriptorSetCommand( VkDevice device
		, VkDescriptorSet descriptorSet
		, VkPipelineLayout layout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_descriptorSet{ static_cast< VkDescriptorSet >( descriptorSet ) }
		, m_layout{ static_cast< VkPipelineLayout >( layout ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicOffsets{ dynamicOffsets }
	{
		assert( get( m_descriptorSet )->getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindDescriptorSetCommand" );
		for ( auto & write : get( m_descriptorSet )->getCombinedTextureSamplers() )
		{
			bindCombinedSampler( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getSamplers() )
		{
			bindSampler( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getSampledTextures() )
		{
			bindSampledTexture( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getStorageTextures() )
		{
			bindStorageTexture( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getUniformBuffers() )
		{
			bindUniformBuffer( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getStorageBuffers() )
		{
			bindStorageBuffer( context, write );
		}

		for ( auto & write : get( m_descriptorSet )->getTexelBuffers() )
		{
			bindTexelBuffer( context, write );
		}

		bindDynamicBuffers( context
			, get( m_descriptorSet )->getDynamicBuffers()
			, m_dynamicOffsets );
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
