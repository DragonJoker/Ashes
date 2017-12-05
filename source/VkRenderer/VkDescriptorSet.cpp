#include "VkDescriptorSet.hpp"

#include "VkBuffer.hpp"
#include "VkDescriptorSetLayoutBinding.hpp"
#include "VkDescriptorSetPool.hpp"
#include "VkSampler.hpp"
#include "VkTexture.hpp"
#include "VkUniformBuffer.hpp"

namespace vk_renderer
{
	DescriptorSet::DescriptorSet( renderer::DescriptorSetPool const & pool )
		: renderer::DescriptorSet{ pool }
		, m_descriptorSet{ static_cast< DescriptorSetPool const & >( pool ).getPool().createDescriptorSet() }
	{
	}

	renderer::CombinedTextureSamplerBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view
		, renderer::Sampler const & sampler )
	{
		m_descriptorSet->createBinding( convert( layoutBinding )
			, static_cast< Texture const & >( view ).getImage().getView()
			, static_cast< Sampler const & >( sampler ).getSampler() );
		return renderer::CombinedTextureSamplerBinding
		{
			layoutBinding,
			view,
			sampler
		};
	}

	renderer::SampledTextureBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view )
	{
		m_descriptorSet->createBinding( convert( layoutBinding )
			, static_cast< Texture const & >( view ).getImage().getView() );
		return renderer::SampledTextureBinding
		{
			layoutBinding,
			view
		};
	}

	renderer::UniformBufferBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
	{
		offset = uniformBuffer.getOffset( offset );
		m_descriptorSet->createBinding( convert( layoutBinding )
			, static_cast< BufferBase const & >( uniformBuffer.getBuffer() ).getBuffer()
			, offset
			, uniformBuffer.getSize() );
		return renderer::UniformBufferBinding
		{
			layoutBinding,
			uniformBuffer,
			offset
		};
	}
	void DescriptorSet::update()const
	{
		m_descriptorSet->update();
	}
}
