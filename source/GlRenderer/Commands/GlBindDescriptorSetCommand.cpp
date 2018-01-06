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
				, static_cast< Texture const & >( binding.getTexture() ).getImage() );
		}

		void bind( renderer::SampledTextureBinding const & binding )
		{
			glActiveTexture( GL_TEXTURE0 + binding.getBinding().getBindingPoint() );
			glBindTexture( GL_TEXTURE_2D
				, static_cast< Texture const & >( binding.getTexture() ).getImage() );
		}

		void bind( renderer::UniformBufferBinding const & binding )
		{
			glBindBufferBase( GL_UNIFORM_BUFFER
				, binding.getBinding().getBindingPoint()
				, static_cast< BufferBase const & >( binding.getUniformBuffer().getBuffer() ).getBuffer() );
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

		for ( auto & descriptor : m_descriptorSet.getSampledTextures() )
		{
			bind( descriptor );
		}

		for ( auto & descriptor : m_descriptorSet.getUniformBuffers() )
		{
			bind( descriptor );
		}
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
