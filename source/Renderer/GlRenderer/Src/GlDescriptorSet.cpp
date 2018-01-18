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
		, renderer::TextureView const & view
		, renderer::Sampler const & sampler )
	{
		m_combinedTextureSamplers.emplace_back( layoutBinding
			, view
			, sampler );
		return m_combinedTextureSamplers.back();
	}

	renderer::SamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Sampler const & sampler )
	{
		m_samplers.emplace_back( layoutBinding
			, sampler );
		return m_samplers.back();
	}

	renderer::SampledTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::ImageLayout layout )
	{
		m_sampledTextures.emplace_back( layoutBinding
			, view
			, layout );
		return m_sampledTextures.back();
	}

	renderer::StorageTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view )
	{
		m_storageTextures.emplace_back( layoutBinding
			, view );
		return m_storageTextures.back();
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

	renderer::StorageBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset )
	{
		m_storageBuffers.emplace_back( layoutBinding
			, storageBuffer
			, offset );
		return m_storageBuffers.back();
	}

	renderer::UniformTexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, renderer::BufferView const & view )
	{
		m_uniformTexelBuffers.emplace_back( layoutBinding
			, uniformBuffer
			, view );
		return m_uniformTexelBuffers.back();
	}

	renderer::StorageTexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, renderer::BufferView const & view )
	{
		m_storageTexelBuffers.emplace_back( layoutBinding
			, storageBuffer
			, view );
		return m_storageTexelBuffers.back();
	}

	void DescriptorSet::update()const
	{
	}
}
