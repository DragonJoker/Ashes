/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindDescriptorSetCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"
#include "Buffer/GlUniformBuffer.hpp"

#include <Descriptor/DescriptorSetLayoutBinding.hpp>

namespace gl_renderer
{
	namespace
	{
		void bind( renderer::CombinedTextureSamplerBinding const & binding )
		{
			glLogCall( gl::ActiveTexture
				, GlTextureUnit( GL_TEXTURE0 + binding.getBinding().getBindingPoint() + binding.getIndex() ) );
			glLogCall( gl::BindTexture
				, convert( binding.getView().getType() )
				, static_cast< TextureView const & >( binding.getView() ).getImage() );
			glLogCall( gl::BindSampler
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Sampler const & >( binding.getSampler() ).getSampler() );
		}

		void bind( renderer::SamplerBinding const & binding )
		{
			glLogCall( gl::BindSampler
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Sampler const & >( binding.getSampler() ).getSampler() );
		}

		void bind( renderer::SampledTextureBinding const & binding )
		{
			glLogCall( gl::ActiveTexture
				, GlTextureUnit( GL_TEXTURE0 + binding.getBinding().getBindingPoint() + binding.getIndex() ) );
			glLogCall( gl::BindTexture
				, convert( binding.getView().getType() )
				, static_cast< TextureView const & >( binding.getView() ).getImage() );
		}

		void bind( renderer::StorageTextureBinding const & binding )
		{
			auto & view = binding.getView();
			auto & range = view.getSubResourceRange();
			glLogCall( gl::ActiveTexture
				, GlTextureUnit( GL_TEXTURE0 + binding.getBinding().getBindingPoint() + binding.getIndex() ) );
			glLogCall( gl::BindImageTexture
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< TextureView const & >( view ).getImage()
				, range.baseMipLevel
				, range.layerCount
				, range.baseArrayLayer
				, GL_ACCESS_TYPE_READ_WRITE
				, getInternal( view.getFormat() ) );
		}

		void bind( renderer::UniformBufferBinding const & binding )
		{
			glLogCall( gl::BindBufferRange
				, GL_BUFFER_TARGET_UNIFORM
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Buffer const & >( binding.getUniformBuffer().getBuffer() ).getBuffer()
				, binding.getOffset()
				, binding.getRange() );
		}

		void bind( renderer::StorageBufferBinding const & binding )
		{
			glLogCall( gl::BindBufferRange
				, GL_BUFFER_TARGET_SHADER_STORAGE
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Buffer const & >( binding.getBuffer() ).getBuffer()
				, binding.getOffset()
				, binding.getRange() );
		}

		void bind( renderer::TexelBufferBinding const & binding )
		{
			glLogCall( gl::ActiveTexture
				, GlTextureUnit( GL_TEXTURE0 + binding.getBinding().getBindingPoint() + binding.getIndex() ) );
			glLogCall( gl::BindTexture
				, GL_BUFFER_TARGET_TEXTURE
				, static_cast< BufferView const & >( binding.getView() ).getImage() );
		}

		void bind( renderer::DynamicUniformBufferBinding const & binding, uint32_t offset )
		{
			glLogCall( gl::BindBufferRange
				, GL_BUFFER_TARGET_UNIFORM
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Buffer const & >( binding.getUniformBuffer().getBuffer() ).getBuffer()
				, binding.getOffset() + offset
				, binding.getRange() );
		}

		void bind( renderer::DynamicStorageBufferBinding const & binding, uint32_t offset )
		{
			glLogCall( gl::BindBufferRange
				, GL_BUFFER_TARGET_SHADER_STORAGE
				, binding.getBinding().getBindingPoint() + binding.getIndex()
				, static_cast< Buffer const & >( binding.getBuffer() ).getBuffer()
				, binding.getOffset() + offset
				, binding.getRange() );
		}

		void bind( std::vector< std::reference_wrapper< renderer::DescriptorSetBinding > > const & bindings
			, renderer::UInt32Array const & offsets )
		{
			for ( auto i = 0u; i < offsets.size(); ++i )
			{
				auto & binding = bindings[i].get();

				switch ( binding.getBinding().getDescriptorType() )
				{
				case renderer::DescriptorType::eUniformBufferDynamic:
					bind( static_cast< renderer::DynamicUniformBufferBinding const & >( binding ), offsets[i] );
					break;

				case renderer::DescriptorType::eStorageBufferDynamic:
					bind( static_cast< renderer::DynamicStorageBufferBinding const & >( binding ), offsets[i] );
					break;

				default:
					assert( false && "Unsupported dynamic descriptor type" );
					throw std::runtime_error{ "Unsupported dynamic descriptor type" };
					break;
				}
			}
		}
	}

	BindDescriptorSetCommand::BindDescriptorSetCommand( renderer::DescriptorSet const & descriptorSet
		, renderer::PipelineLayout const & layout
		, renderer::UInt32Array const & dynamicOffsets
		, renderer::PipelineBindPoint bindingPoint )
		: m_descriptorSet{ static_cast< DescriptorSet const & >( descriptorSet ) }
		, m_layout{ static_cast< PipelineLayout const & >( layout ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicOffsets{ dynamicOffsets }
	{
		assert( m_descriptorSet.getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply()const
	{
		glLogCommand( "BindDescriptorSetCommand" );
		for ( auto & descriptor : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getSamplers() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getSampledTextures() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getStorageTextures() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getUniformBuffers() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getStorageBuffers() )
		{
			bind( *descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getTexelBuffers() )
		{
			bind( *descriptor );
		}

		bind( m_descriptorSet.getDynamicBuffers(), m_dynamicOffsets );
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
