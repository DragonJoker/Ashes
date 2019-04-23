/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindDescriptorSetCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Descriptor/DescriptorSetLayoutBinding.hpp>

namespace gl_renderer
{
	namespace
	{
		static constexpr GLenum GL_TEXTURE_FILTER_CONTROL = 0x8500;
		static constexpr GLenum GL_TEXTURE_LOD_BIAS = 0x8501;

		ashes::ImageView const & getView( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return write.imageInfo[index].imageView.value().get();
		}

		ashes::Sampler const & getSampler( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.imageInfo.size() );
			return write.imageInfo[index].sampler.value().get();
		}

		ashes::BufferBase const & getBuffer( ashes::WriteDescriptorSet const & write, uint32_t index )
		{
			assert( index < write.bufferInfo.size() );
			return write.bufferInfo[index].buffer.get();
		}

		GlTextureType doBindTextureView( ContextLock const & context
			, ashes::ImageView const & view
			, uint32_t bindingIndex )
		{
			auto target = convert( view.getType(), view.getImage().getLayerCount() );
			glLogCall( context
				, glActiveTexture
				, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
			glLogCall( context
				, glBindTexture
				, target
				, static_cast< Image const & >( view.getImage() ).getImage() );

			if ( view.getComponentMapping().r != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, target
					, GL_TEX_PARAMETER_SWIZZLE_R
					, convert( view.getComponentMapping().r ) );
			}

			if ( view.getComponentMapping().g != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, target
					, GL_TEX_PARAMETER_SWIZZLE_G
					, convert( view.getComponentMapping().g ) );
			}

			if ( view.getComponentMapping().b != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, target
					, GL_TEX_PARAMETER_SWIZZLE_B
					, convert( view.getComponentMapping().b ) );
			}

			if ( view.getComponentMapping().a != VK_COMPONENT_SWIZZLE_IDENTITY )
			{
				glLogCall( context
					, glTexParameteri
					, target
					, GL_TEX_PARAMETER_SWIZZLE_A
					, convert( view.getComponentMapping().a ) );
			}

			return target;
		}

		void doBindSampler( ContextLock const & context
			, ashes::Sampler const & sampler
			, uint32_t bindingIndex )
		{
			glLogCall( context
				, glBindSampler
				, bindingIndex
				, static_cast< Sampler const & >( sampler ).getSampler() );
		}

		void bindCombinedSampler( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & view = getView( write, i );
				auto & sampler = getSampler( write, i );
				auto target = doBindTextureView( context, view, bindingIndex );
				doBindSampler( context, sampler, bindingIndex );
				glLogCall( context
					, glTexParameterf
					, target
					, GL_TEX_PARAMETER_LOD_BIAS
					, sampler.getLodBias() );
			}
		}

		void bindSampler( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & sampler = getSampler( write, i );
				doBindSampler( context, sampler, bindingIndex );
			}
		}

		void bindSampledTexture( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & view = getView( write, i );
				doBindTextureView( context, view, bindingIndex );
			}
		}

		void bindStorageTexture( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.imageInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & view = getView( write, i );
				auto & range = view.getSubResourceRange();
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindImageTexture_ARB
					, bindingIndex
					, static_cast< Image const & >( view.getImage() ).getImage()
					, range.baseMipLevel
					, range.layerCount
					, range.baseArrayLayer
					, GL_ACCESS_TYPE_READ_WRITE
					, getInternal( view.getFormat() ) );
			}
		}

		void bindUniformBuffer( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_UNIFORM
					, bindingIndex
					, static_cast< Buffer const & >( buffer ).getBuffer()
					, GLintptr( write.bufferInfo[i].offset )
					, GLsizeiptr( std::min( write.bufferInfo[i].range, uint64_t( buffer.getSize() ) ) ) );
			}
		}

		void bindStorageBuffer( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, bindingIndex
					, static_cast< Buffer const & >( buffer ).getBuffer()
					, GLintptr( write.bufferInfo[i].offset )
					, GLsizeiptr( std::min( write.bufferInfo[i].range, uint64_t( buffer.getSize() ) ) ) );
			}
		}

		void bindTexelBuffer( ContextLock const & context
			, ashes::WriteDescriptorSet const & write )
		{
			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & buffer = getBuffer( write, i );
				glLogCall( context
					, glActiveTexture
					, GlTextureUnit( GL_TEXTURE0 + bindingIndex ) );
				glLogCall( context
					, glBindTexture
					, GL_BUFFER_TARGET_TEXTURE
					, static_cast< BufferView const & >( write.texelBufferView[i].get() ).getImage() );
			}
		}

		void bindDynamicUniformBuffer( ContextLock const & context
			, ashes::WriteDescriptorSet const & write
			, uint32_t offset )
		{
			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_UNIFORM
					, bindingIndex
					, static_cast< Buffer const & >( buffer ).getBuffer()
					, GLintptr( write.bufferInfo[i].offset + offset )
					, GLsizeiptr( std::min( write.bufferInfo[i].range, uint64_t( buffer.getSize() ) ) ) );
			}
		}

		void bindDynamicStorageBuffer( ContextLock const & context
			, ashes::WriteDescriptorSet const & write
			, uint32_t offset )
		{
			for ( auto i = 0u; i < write.bufferInfo.size(); ++i )
			{
				uint32_t bindingIndex = write.dstBinding + write.dstArrayElement + i;
				auto & buffer = getBuffer( write, i );
				glLogCall( context
					, glBindBufferRange
					, GL_BUFFER_TARGET_SHADER_STORAGE
					, bindingIndex
					, static_cast< Buffer const & >( buffer ).getBuffer()
					, GLintptr( write.bufferInfo[i].offset + offset )
					, GLsizeiptr( std::min( write.bufferInfo[i].range, uint64_t( buffer.getSize() ) ) ) );
			}
		}

		void bindDynamicBuffers( ContextLock const & context
			, ashes::VkWriteDescriptorSetArray const & writes
			, ashes::UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & write = writes[i];

				switch ( write.descriptorType )
				{
				case ashes::DescriptorType::eUniformBufferDynamic:
					bindDynamicUniformBuffer( context, write, offsets[i] );
					break;

				case ashes::DescriptorType::eStorageBufferDynamic:
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

	BindDescriptorSetCommand::BindDescriptorSetCommand( Device const & device
		, ashes::DescriptorSet const & descriptorSet
		, ashes::PipelineLayout const & layout
		, ashes::UInt32Array const & dynamicOffsets
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_descriptorSet{ static_cast< DescriptorSet const & >( descriptorSet ) }
		, m_layout{ static_cast< PipelineLayout const & >( layout ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicOffsets{ dynamicOffsets }
	{
		assert( m_descriptorSet.getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindDescriptorSetCommand" );
		for ( auto & write : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bindCombinedSampler( context, write );
		}

		for ( auto & write : m_descriptorSet.getSamplers() )
		{
			bindSampler( context, write );
		}

		for ( auto & write : m_descriptorSet.getSampledTextures() )
		{
			bindSampledTexture( context, write );
		}

		for ( auto & write : m_descriptorSet.getStorageTextures() )
		{
			bindStorageTexture( context, write );
		}

		for ( auto & write : m_descriptorSet.getUniformBuffers() )
		{
			bindUniformBuffer( context, write );
		}

		for ( auto & write : m_descriptorSet.getStorageBuffers() )
		{
			bindStorageBuffer( context, write );
		}

		for ( auto & write : m_descriptorSet.getTexelBuffers() )
		{
			bindTexelBuffer( context, write );
		}

		bindDynamicBuffers( context, m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
