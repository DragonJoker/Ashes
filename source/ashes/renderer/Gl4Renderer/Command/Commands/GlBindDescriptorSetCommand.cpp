/*
This file belongs to Ashes.
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
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, i );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType()
						, get( view )->getSubresourceRange().layerCount
						, get( get( view )->getImage() )->getSamples() )
					, dstBinding + write.dstArrayElement + i
					, list );

				auto sampler = getSampler( write, i );
				bindSampler( get( sampler )->getInternal()
					, dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindSampler( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto sampler = getSampler( write, i );
				bindSampler( get( sampler )->getInternal()
					, dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindSampledTexture( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, i );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType()
						, get( view )->getSubresourceRange().layerCount
						, get( get( view )->getImage() )->getSamples() )
					, dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindInputAttachment( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, VkSampler sampler
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				auto view = getView( write, 0u );
				bindImage( get( view )->getInternal()
					, convertViewType( get( view )->getType()
						, get( view )->getSubresourceRange().layerCount
						, get( get( view )->getImage() )->getSamples() )
					, dstBinding + write.dstArrayElement + i
					, list );
				bindSampler( get( sampler )->getInternal()
					, dstBinding + write.dstArrayElement + i
					, list );
			}
		}

		void bindStorageTexture( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
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
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_UNIFORM
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindStorageBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindTexelBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );

				list.push_back( makeCmd< OpType::eBindTexture >( GL_BUFFER_TARGET_TEXTURE
					, get( write.pTexelBufferView[i] )->getImage() ) );
			}
		}

		void bindDynamicUniformBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_UNIFORM
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset + offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindDynamicStorageBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset
			, CmdList & list )
		{
			auto it = bindings.find( makeShaderBindingKey( setIndex, write.dstBinding ) );
			assert( it != bindings.end() );
			auto dstBinding = it->second;

			for ( auto i = 0u; i < write.descriptorCount; ++i )
			{
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto buffer = getBuffer( write, i );
				list.push_back( makeCmd< OpType::eBindBufferRange >( bindingIndex
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, get( buffer )->getInternal()
					, int64_t( get( buffer )->getInternalOffset() + write.pBufferInfo[i].offset + offset )
					, int64_t( std::min( write.pBufferInfo[i].range, uint64_t( get( buffer )->getMemoryRequirements().size ) ) ) ) );
			}
		}

		void bindCombinedSampler( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindCombinedSampler( write, bindings, setIndex, list );
			}
		}

		void bindSampler( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindSampler( write, bindings, setIndex, list );
			}
		}

		void bindSampledTexture( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindSampledTexture( write, bindings, setIndex, list );
			}
		}

		void bindInputAttachment( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, VkSampler sampler
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindInputAttachment( write, bindings, setIndex, sampler, list );
			}
		}

		void bindStorageTexture( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageTexture( write, bindings, setIndex, list );
			}
		}

		void bindUniformBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindUniformBuffer( write, bindings, setIndex, list );
			}
		}

		void bindStorageBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindStorageBuffer( write, bindings, setIndex, list );
			}
		}

		void bindTexelBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindTexelBuffer( write, bindings, setIndex, list );
			}
		}

		void bindDynamicUniformBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicUniformBuffer( write, bindings, setIndex, offset, list );
			}
		}

		void bindDynamicStorageBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, uint32_t offset
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindDynamicStorageBuffer( write, bindings, setIndex, offset, list );
			}
		}

		void bindDynamicBuffers( LayoutBindingWritesArray const & writes
			, ShaderBindings const & bindings
			, uint32_t setIndex
			, UInt32Array const & offsets
			, CmdList & list )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & write = writes[i];

				switch ( write->descriptorType )
				{
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					bindDynamicUniformBuffer( write, bindings.ubo, setIndex, offsets[i], list );
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
					bindDynamicStorageBuffer( write, bindings.sbo, setIndex, offsets[i], list );
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
		, VkPipelineLayout pipelineLayout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		assert( get( descriptorSet )->getDynamicBuffers().size() == dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
		glLogCommand( "BindDescriptorSetCommand" );
		auto & bindings = get( pipelineLayout )->getShaderBindings();
		auto setIndex = get( pipelineLayout )->getDescriptorSetIndex( descriptorSet );

		if ( setIndex != GL_INVALID_INDEX )
		{
			for ( auto & write : get( descriptorSet )->getInputAttachments() )
			{
				bindInputAttachment( write, bindings.tex, setIndex, get( device )->getSampler(), list );
			}

			for ( auto & write : get( descriptorSet )->getCombinedTextureSamplers() )
			{
				bindCombinedSampler( write, bindings.tex, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getSamplers() )
			{
				bindSampler( write, bindings.tex, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getSampledTextures() )
			{
				bindSampledTexture( write, bindings.tex, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getStorageTextures() )
			{
				bindStorageTexture( write, bindings.img, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getUniformBuffers() )
			{
				bindUniformBuffer( write, bindings.ubo, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getInlineUniforms() )
			{
				bindUniformBuffer( write, bindings.ubo, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getStorageBuffers() )
			{
				bindStorageBuffer( write, bindings.sbo, setIndex, list );
			}

			for ( auto & write : get( descriptorSet )->getTexelBuffers() )
			{
				bindTexelBuffer( write, bindings.tbo, setIndex, list );
			}

			bindDynamicBuffers( get( descriptorSet )->getDynamicBuffers()
				, bindings
				, setIndex
				, dynamicOffsets
				, list );
		}
	}
}
