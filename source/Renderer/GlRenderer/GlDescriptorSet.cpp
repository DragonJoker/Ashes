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

	renderer::CombinedTextureSamplerBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view
		, renderer::Sampler const & sampler )
	{
		renderer::CombinedTextureSamplerBinding result
		{
			layoutBinding,
			view,
			sampler
		};
		m_combinedTextureSamplers.push_back( result );
		return result;
	}

	renderer::SampledTextureBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view )
	{
		renderer::SampledTextureBinding result
		{
			layoutBinding,
			view
		};
		m_sampledTextures.push_back( result );
		return result;
	}

	renderer::UniformBufferBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
	{
		renderer::UniformBufferBinding result
		{
			layoutBinding,
			uniformBuffer,
			offset
		};
		m_uniformBuffers.push_back( result );
		return result;
	}

	void DescriptorSet::update()const
	{
	}
}
