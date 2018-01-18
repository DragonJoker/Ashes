/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindDescriptorSetCommand.hpp"

#include "GlRenderer/GlBuffer.hpp"
#include "GlRenderer/GlDescriptorSet.hpp"
#include "GlRenderer/GlPipelineLayout.hpp"
#include "GlRenderer/GlSampler.hpp"
#include "GlRenderer/GlTexture.hpp"
#include "GlRenderer/GlTextureView.hpp"
#include "GlRenderer/GlUniformBuffer.hpp"

#include <Renderer/DescriptorSetLayoutBinding.hpp>

namespace gl_renderer
{
	namespace
	{
		void bind( renderer::CombinedTextureSamplerBinding const & binding )
		{
			glActiveTexture( GL_TEXTURE0 + binding.getBinding().getBindingPoint() );
			glBindSampler( binding.getBinding().getBindingPoint()
				, static_cast< Sampler const & >( binding.getSampler() ).getSampler() );
			glBindTexture( GL_TEXTURE_2D
				, static_cast< Texture const & >( binding.getTexture().getImage() ).getImage() );
		}

		void bind( renderer::SamplerBinding const & binding )
		{
			glBindSampler( binding.getBinding().getBindingPoint()
				, static_cast< Sampler const & >( binding.getSampler() ).getSampler() );
		}

		void bind( renderer::SampledTextureBinding const & binding )
		{
			glActiveTexture( GL_TEXTURE0 + binding.getBinding().getBindingPoint() );
			glBindTexture( GL_TEXTURE_2D
				, static_cast< Texture const & >( binding.getView().getImage() ).getImage() );
		}

		void bind( renderer::StorageTextureBinding const & binding )
		{
			auto & view = binding.getView();
			auto & range = view.getSubResourceRange();
			glActiveTexture( GL_TEXTURE0 + binding.getBinding().getBindingPoint() );
			glBindImageTexture( binding.getBinding().getBindingPoint()
				, static_cast< Texture const & >( view.getImage() ).getImage()
				, range.getBaseMipLevel()
				, range.getLayerCount() > 0
				, range.getBaseArrayLayer()
				, GL_READ_WRITE
				, getFormat( view.getFormat() ) );
		}

		void bind( renderer::UniformBufferBinding const & binding )
		{
			glBindBufferBase( GL_UNIFORM_BUFFER
				, binding.getBinding().getBindingPoint()
				, static_cast< Buffer const & >( binding.getUniformBuffer().getBuffer() ).getBuffer() );
		}

		void bind( renderer::StorageBufferBinding const & binding )
		{
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER
				, binding.getBinding().getBindingPoint()
				, static_cast< Buffer const & >( binding.getBuffer() ).getBuffer() );
		}

		void bind( renderer::UniformTexelBufferBinding const & binding )
		{
			glBindBufferBase( GL_TEXTURE_BUFFER
				, binding.getBinding().getBindingPoint()
				, static_cast< Buffer const & >( binding.getBuffer().getBuffer() ).getBuffer() );
		}

		void bind( renderer::StorageTexelBufferBinding const & binding )
		{
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER
				, binding.getBinding().getBindingPoint()
				, static_cast< Buffer const & >( binding.getBuffer() ).getBuffer() );
		}
	}

	BindDescriptorSetCommand::BindDescriptorSetCommand( renderer::DescriptorSet const & descriptorSet
		, renderer::PipelineLayout const & layout
		, renderer::PipelineBindPoint bindingPoint )
		: m_descriptorSet{ static_cast< DescriptorSet const & >( descriptorSet ) }
		, m_layout{ static_cast< PipelineLayout const & >( layout ) }
		, m_bindingPoint{ bindingPoint }
	{
	}

	void BindDescriptorSetCommand::apply()const
	{
		for ( auto & descriptor : m_descriptorSet.getCombinedTextureSamplers() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getSamplers() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getSampledTextures() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getStorageTextures() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getUniformBuffers() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getStorageBuffers() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getUniformTexelBuffers() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getStorageTexelBuffers() )
		{
			bind( descriptor );
		}
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
