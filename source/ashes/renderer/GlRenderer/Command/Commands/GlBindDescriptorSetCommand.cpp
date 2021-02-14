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

#include "ashesgl_api.hpp"

namespace ashes::gl
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

	void apply( ContextLock const & context
		, CmdTexParameteri const & cmd )
	{
		glLogCall( context
			, glTexParameteri
			, cmd.target
			, cmd.name
			, cmd.param );
	}

	void apply( ContextLock const & context
		, CmdTexParameterf const & cmd )
	{
		glLogCall( context
			, glTexParameterf
			, cmd.target
			, cmd.name
			, cmd.param );
	}

	namespace common
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

		void bindSampler( VkSampler sampler
			, uint32_t bindingIndex
			, CmdList & list )
		{
			list.push_back( makeCmd< OpType::eBindSampler >( bindingIndex, get( sampler )->getInternal() ) );
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

				list.push_back( makeCmd< OpType::eBindTexture >( GL_TEXTURE_BUFFER
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
			, ArrayView< uint32_t const > const & offsets
			, uint32_t & dynamicOffsetIndex
			, CmdList & list )
		{
			for ( auto & write : writes )
			{
				if ( write->descriptorCount )
				{
					switch ( write->descriptorType )
					{
					case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
						bindDynamicUniformBuffer( write, bindings.ubo, setIndex, offsets[dynamicOffsetIndex], list );
						break;

					case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
						bindDynamicStorageBuffer( write, bindings.sbo, setIndex, offsets[dynamicOffsetIndex], list );
						break;

					default:
						assert( false && "Unsupported dynamic descriptor type" );
						throw std::runtime_error{ "Unsupported dynamic descriptor type" };
						break;
					}

					++dynamicOffsetIndex;
				}
			}
		}
	}

	namespace gl3
	{
		void setLodBias( VkSampler sampler
			, GlTextureType target
			, CmdList & list )
		{
			list.push_back( makeCmd< OpType::eTexParameterf >( target
				, uint32_t( GL_TEX_PARAMETER_LOD_BIAS )
				, get( sampler )->getLodBias() ) );
		}

		GlTextureType bindTexture( VkImageView view
			, uint32_t bindingIndex
			, CmdList & list )
		{
			auto target = convertViewType( get( view )->getType()
				, get( get( view )->getImage() )->getType()
				, get( get( view )->getImage() )->getArrayLayers()
				, get( get( view )->getImage() )->getSamples() );
			list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );
			list.push_back( makeCmd< OpType::eBindTexture >( target
				, get( get( view )->getImage() )->getInternal() ) );

			if ( get( view )->getComponents().r != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				list.push_back( makeCmd< OpType::eTexParameteri >( target
					, uint32_t( GL_TEX_PARAMETER_SWIZZLE_R )
					, int32_t( convertComponentSwizzle( get( view )->getComponents().r ) ) ) );
			}

			if ( get( view )->getComponents().g != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				list.push_back( makeCmd< OpType::eTexParameteri >( target
					, uint32_t( GL_TEX_PARAMETER_SWIZZLE_G )
					, int32_t( convertComponentSwizzle( get( view )->getComponents().g ) ) ) );
			}

			if ( get( view )->getComponents().b != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				list.push_back( makeCmd< OpType::eTexParameteri >( target
					, uint32_t( GL_TEX_PARAMETER_SWIZZLE_B )
					, int32_t( convertComponentSwizzle( get( view )->getComponents().b ) ) ) );
			}

			if ( get( view )->getComponents().a != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				list.push_back( makeCmd< OpType::eTexParameteri >( target
					, uint32_t( GL_TEX_PARAMETER_SWIZZLE_A )
					, int32_t( convertComponentSwizzle( get( view )->getComponents().a ) ) ) );
			}

			return target;
		}

		void bindImage( VkImageView view
			, uint32_t bindingIndex
			, CmdList & list )
		{
			auto name = get( get( view )->getImage() )->getInternal();
			auto & range = get( view )->getSubresourceRange();
			list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );
			list.push_back( makeCmd< OpType::eBindImage >( bindingIndex
				, name
				, range.baseMipLevel
				, range.layerCount
				, range.baseArrayLayer
				, getInternalFormat( get( view )->getFormat() ) ) );
		}

		void bindTextureAndSampler( VkImageView view
			, VkSampler sampler
			, uint32_t bindingIndex
			, CmdList & list )
		{
			auto target = bindTexture( view
				, bindingIndex
				, list );
			common::bindSampler( sampler
				, bindingIndex
				, list );
			setLodBias( sampler
				, target
				, list );
		}

		void bindCombinedSampler( VkWriteDescriptorSet const & write
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto writeSampler = common::getSampler( write, i );

				if ( write.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
					|| writeSampler == VK_NULL_HANDLE )
				{
					bindTextureAndSampler( common::getView( write, i )
						, sampler
						, bindingIndex
						, list );
				}
				else
				{
					bindTextureAndSampler( common::getView( write, i )
						, writeSampler
						, bindingIndex
						, list );
				}
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
				common::bindSampler( common::getSampler( write, i )
					, bindingIndex
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				bindTexture( common::getView( write, i )
					, bindingIndex
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				bindTextureAndSampler( common::getView( write, i )
					, sampler
					, bindingIndex
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
				bindImage( common::getView( write, i )
					, bindingIndex
					, list );
			}
		}

		void bindImageBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			common::bindTexelBuffer( write, bindings, setIndex, list );
		}

		void bindCombinedSampler( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, VkSampler sampler
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindCombinedSampler( write, bindings, setIndex, sampler, list );
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

		void bindImageBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindImageBuffer( write, bindings, setIndex, list );
			}
		}
	}

	namespace gl4
	{
		GlTextureType bindTexture( VkImageView view
			, uint32_t bindingIndex
			, CmdList & list )
		{
			auto name = get( view )->getInternal();
			auto type = getTextureType( get( view )->getType()
				, get( view )->getSubresourceRange().layerCount
				, get( get( view )->getImage() )->getSamples() );
			list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );
			list.push_back( makeCmd< OpType::eBindTexture >( type, name ) );
			return GlTextureType{};
		}

		void bindImage( VkImageView view
			, uint32_t bindingIndex
			, CmdList & list )
		{
			auto name = get( view )->getInternal();
			auto & range = get( view )->getSubresourceRange();
			list.push_back( makeCmd< OpType::eActiveTexture >( bindingIndex ) );
			list.push_back( makeCmd< OpType::eBindImage >( bindingIndex
				, name
				, range.baseMipLevel
				, range.layerCount
				, range.baseArrayLayer
				, getInternalFormat( get( view )->getFormat() ) ) );
		}

		void bindTextureAndSampler( VkImageView view
			, VkSampler sampler
			, uint32_t bindingIndex
			, CmdList & list )
		{
			bindTexture( view
				, bindingIndex
				, list );
			common::bindSampler( sampler
				, bindingIndex
				, list );
		}

		void bindCombinedSampler( VkWriteDescriptorSet const & write
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				auto writeSampler = common::getSampler( write, i );

				if ( write.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
					|| writeSampler == VK_NULL_HANDLE )
				{
					bindTextureAndSampler( common::getView( write, i )
						, sampler
						, bindingIndex
						, list );
				}
				else
				{

					bindTextureAndSampler( common::getView( write, i )
						, writeSampler
						, bindingIndex
						, list );
				}
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
				common::bindSampler( common::getSampler( write, i )
					, bindingIndex
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				bindTexture( common::getView( write, i )
					, bindingIndex
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
				uint32_t bindingIndex = dstBinding + write.dstArrayElement + i;
				bindTextureAndSampler( common::getView( write, i )
					, sampler
					, bindingIndex
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
				bindImage( common::getView( write, i )
					, bindingIndex
					, list );
			}
		}

		void bindImageBuffer( VkWriteDescriptorSet const & write
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			common::bindTexelBuffer( write, bindings, setIndex, list );
		}

		void bindCombinedSampler( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, VkSampler sampler
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindCombinedSampler( write, bindings, setIndex, sampler, list );
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

		void bindImageBuffer( LayoutBindingWrites const * writes
			, ShaderBindingMap const & bindings
			, uint32_t setIndex
			, CmdList & list )
		{
			for ( auto & write : writes->writes )
			{
				bindImageBuffer( write, bindings, setIndex, list );
			}
		}
	}

	void buildBindDescriptorSetCommand( VkDevice device
		, VkDescriptorSet descriptorSet
		, uint32_t setIndex
		, VkPipelineLayout pipelineLayout
		, ArrayView< uint32_t const > const & dynamicOffsets
		, uint32_t & dynamicOffsetIndex
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		assert( get( descriptorSet )->getDynamicBuffers().size() + dynamicOffsetIndex <= dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
		glLogCommand( list, "BindDescriptorSetCommand" );

		auto & bindings = get( pipelineLayout )->getDescriptorSetBindings( descriptorSet, setIndex );

		if ( setIndex != GL_INVALID_INDEX )
		{
			if ( hasTextureViews( device ) )
			{
				for ( auto & write : get( descriptorSet )->getInputAttachments() )
				{
					gl4::bindInputAttachment( write, bindings.tex, setIndex, get( device )->getSampler(), list );
				}

				for ( auto & write : get( descriptorSet )->getCombinedTextureSamplers() )
				{
					gl4::bindCombinedSampler( write, bindings.tex, setIndex, get( device )->getSampler(), list );
				}

				for ( auto & write : get( descriptorSet )->getSamplers() )
				{
					gl4::bindSampler( write, bindings.tex, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getSampledTextures() )
				{
					gl4::bindSampledTexture( write, bindings.tex, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getStorageTextures() )
				{
					gl4::bindStorageTexture( write, bindings.img, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getTexelImageBuffers() )
				{
					gl4::bindImageBuffer( write, bindings.ibo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getUniformBuffers() )
				{
					common::bindUniformBuffer( write, bindings.ubo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getInlineUniforms() )
				{
					common::bindUniformBuffer( write, bindings.ubo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getStorageBuffers() )
				{
					common::bindStorageBuffer( write, bindings.sbo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getTexelSamplerBuffers() )
				{
					common::bindTexelBuffer( write, bindings.tbo, setIndex, list );
				}

				common::bindDynamicBuffers( get( descriptorSet )->getDynamicBuffers()
					, bindings
					, setIndex
					, dynamicOffsets
					, dynamicOffsetIndex
					, list );
			}
			else
			{
				for ( auto & write : get( descriptorSet )->getInputAttachments() )
				{
					gl3::bindInputAttachment( write, bindings.tex, setIndex, get( device )->getSampler(), list );
				}

				for ( auto & write : get( descriptorSet )->getCombinedTextureSamplers() )
				{
					gl3::bindCombinedSampler( write, bindings.tex, setIndex, get( device )->getSampler(), list );
				}

				for ( auto & write : get( descriptorSet )->getSamplers() )
				{
					gl3::bindSampler( write, bindings.tex, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getSampledTextures() )
				{
					gl3::bindSampledTexture( write, bindings.tex, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getStorageTextures() )
				{
					gl3::bindStorageTexture( write, bindings.img, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getTexelImageBuffers() )
				{
					gl3::bindImageBuffer( write, bindings.ibo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getUniformBuffers() )
				{
					common::bindUniformBuffer( write, bindings.ubo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getInlineUniforms() )
				{
					common::bindUniformBuffer( write, bindings.ubo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getStorageBuffers() )
				{
					common::bindStorageBuffer( write, bindings.sbo, setIndex, list );
				}

				for ( auto & write : get( descriptorSet )->getTexelSamplerBuffers() )
				{
					common::bindTexelBuffer( write, bindings.tbo, setIndex, list );
				}

				common::bindDynamicBuffers( get( descriptorSet )->getDynamicBuffers()
					, bindings
					, setIndex
					, dynamicOffsets
					, dynamicOffsetIndex
					, list );
			}
		}
	}
}
