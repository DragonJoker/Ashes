#include "GlDescriptorSet.hpp"

#include "GlBuffer.hpp"
#include "GlDescriptorSetPool.hpp"
#include "GlSampler.hpp"
#include "GlTexture.hpp"
#include "GlUniformBuffer.hpp"

namespace gl_renderer
{
	DescriptorSet::DescriptorSet( renderer::DescriptorSetPool const & pool )
		: renderer::DescriptorSet{ pool }
	{
	}

	renderer::CombinedTextureSamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view
		, renderer::Sampler const & sampler )
	{
		m_combinedTextureSamplers.emplace_back( layoutBinding
			, view
			, sampler );
		return m_combinedTextureSamplers.back();
	}

	renderer::SampledTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view )
	{
		m_sampledTextures.emplace_back( layoutBinding
			, view );
		return m_sampledTextures.back();
	}

	renderer::UniformBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
	{
		m_uniformBuffers.emplace_back( layoutBinding
			, uniformBuffer
			, offset );
		return m_uniformBuffers.back();
	}

	void DescriptorSet::update()const
	{
	}
}
