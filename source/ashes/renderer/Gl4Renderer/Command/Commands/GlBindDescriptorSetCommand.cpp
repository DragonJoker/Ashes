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
	void apply( ContextLock const & context
		, CmdBindSampler const & cmd )
	{
		glLogCall( context
			, glBindSampler
			, cmd.binding
			, cmd.name );
	}

	void apply( ContextLock const & context
		, CmdBindImage const & cmd )
	{
		glLogCall( context
			, glBindImageTexture
			, cmd.binding
			, cmd.name
			, cmd.baseMipLevel
			, cmd.layerCount
			, cmd.baseArrayLayer
			, GL_ACCESS_TYPE_READ_WRITE
			, cmd.internal );
	}

	void apply( ContextLock const & context
		, CmdBindBufferRange const & cmd )
	{
		glLogCall( context
			, glBindBufferRange
			, cmd.target
			, cmd.binding
			, cmd.name
			, GLintptr( cmd.offset )
			, GLsizeiptr( cmd.range ) );
	}

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

		void bindImage( GLuint name
			, GLenum type
			, uint32_t bindingIndex
			, CmdList & list )
		{
			list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );
			list.push_back( makeCmd< OpType::eBindTexture >( type, name ) );
		}

		void bindSampler( GLuint name
			, uint32_t bindingIndex
			, CmdList & list )
		{
			list.push_back( makeCmd< OpType::eBindSampler >( bindingIndex, name ) );
		}

		void bindCombinedSampler( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, i );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType() )
					, write.dstBinding + write.dstArrayElement + i
					, list );

				auto sampler = getSampler( write, i );
				bindSampler( get( sampler )->getInternal()
					, write.dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindSampler( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto sampler = getSampler( write, i );
				bindSampler( get( sampler )->getInternal()
					, write.dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindSampledTexture( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, i );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType() )
					, write.dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindInputAttachment( VkWriteDescriptorSet const & write
			, VkSampler sampler
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, 0u );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType() )
					, write.dstBinding + write.dstArrayElement + i
					, list );
				bindSampler( get( sampler )->getInternal()
					, write.dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindStorageTexture( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );

				auto view = getView( write, i );
				auto & range = get( view )->getSubresourceRange();
				list.push_back( makeCmd< OpType::eBindImage >( bindingIndex
					, get( view )->getInternal()
					, range.baseMipLevel
					, range.layerCount
					, range.baseArrayLayer
					, getInternalFormat( get( view )->getFormat() ) ) );
			}
		}

		void bindUniformBuffer( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_UNIFORM
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindStorageBuffer( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindTexelBuffer( VkWriteDescriptorSet const & write
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );

				list.push_back( makeCmd< OpType::eBindTexture >( GL_BUFFER_TARGET_TEXTURE
					, get( write.pTexelBufferView[i] )->getImage() ) );
			}
		}

		void bindDynamicUniformBuffer( VkWriteDescriptorSet const & write
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_UNIFORM
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset + offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindDynamicStorageBuffer( VkWriteDescriptorSet const & write
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset + offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindCombinedSampler( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindCombinedSampler( write, list );
			}
		}

		void bindSampler( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindSampler( write, list );
			}
		}

		void bindSampledTexture( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindSampledTexture( write, list );
			}
		}

		void bindInputAttachment( LayoutBindingWrites const * writes
			, VkSampler sampler
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindInputAttachment( write, sampler, list );
			}
		}

		void bindStorageTexture( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageTexture( write, list );
			}
		}

		void bindUniformBuffer( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindUniformBuffer( write, list );
			}
		}

		void bindStorageBuffer( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageBuffer( write, list );
			}
		}

		void bindTexelBuffer( LayoutBindingWrites const * writes
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindTexelBuffer( write, list );
			}
		}

		void bindDynamicUniformBuffer( LayoutBindingWrites const * writes
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicUniformBuffer( write, offset, list );
			}
		}

		void bindDynamicStorageBuffer( LayoutBindingWrites const * writes
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicStorageBuffer( write, offset, list );
			}
		}

		void bindDynamicBuffers( LayoutBindingWritesArray const & writes
			, UInt32Array const & offsets
			, CmdList & list )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & write = writes[i];

				switch ( write->descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					bindDynamicUniformBuffer( write, offsets[i], list );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					bindDynamicStorageBuffer( write, offsets[i], list );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}
	}

	void buildBindDescriptorSetCommand( VkDevice device
		, VkDescriptorSet descriptorSet
		, VkPipelineLayout layout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		assert( get( descriptorSet )->getDynamicBuffers().size() == dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
		glLogCommand( "BindDescriptorSetCommand" );

		for ( auto & write : get( descriptorSet )->getInputAttachments() )
		{
			bindInputAttachment( write, get( device )->getSampler(), list );
		}

		for ( auto & write : get( descriptorSet )->getCombinedTextureSamplers() )
		{
			bindCombinedSampler( write, list );
		}

		for ( auto & write : get( descriptorSet )->getSamplers() )
		{
			bindSampler( write, list );
		}

		for ( auto & write : get( descriptorSet )->getSampledTextures() )
		{
			bindSampledTexture( write, list );
		}

		for ( auto & write : get( descriptorSet )->getStorageTextures() )
		{
			bindStorageTexture( write, list );
		}

		for ( auto & write : get( descriptorSet )->getUniformBuffers() )
		{
			bindUniformBuffer( write, list );
		}

		for ( auto & write : get( descriptorSet )->getStorageBuffers() )
		{
			bindStorageBuffer( write, list );
		}

		for ( auto & write : get( descriptorSet )->getTexelBuffers() )
		{
			bindTexelBuffer( write, list );
		}

		bindDynamicBuffers( get( descriptorSet )->getDynamicBuffers()
			, dynamicOffsets
			, list );
	}
}
