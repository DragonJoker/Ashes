#include "Descriptor/GlDescriptorSet.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Descriptor/GlDescriptorSetPool.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlTexture.hpp"
#include "Buffer/GlUniformBuffer.hpp"

#include <Descriptor/DescriptorSetLayoutBinding.hpp>

namespace gl_renderer
{
	DescriptorSet::DescriptorSet( renderer::DescriptorSetPool const & pool, uint32_t bindingPoint )
		: renderer::DescriptorSet{ pool, bindingPoint }
	{
	}

	renderer::CombinedTextureSamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::Sampler const & sampler
		, uint32_t index )
	{
		m_combinedTextureSamplers.emplace_back( std::make_unique< renderer::CombinedTextureSamplerBinding >( layoutBinding
			, view
			, sampler
			, index ) );
		return *m_combinedTextureSamplers.back();
	}

	renderer::SamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Sampler const & sampler
		, uint32_t index )
	{
		m_samplers.emplace_back( std::make_unique< renderer::SamplerBinding >( layoutBinding
			, sampler
			, index ) );
		return *m_samplers.back();
	}

	renderer::SampledTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::ImageLayout layout
		, uint32_t index )
	{
		m_sampledTextures.emplace_back( std::make_unique< renderer::SampledTextureBinding >( layoutBinding
			, view
			, layout
			, index ) );
		return *m_sampledTextures.back();
	}

	renderer::StorageTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, uint32_t index )
	{
		m_storageTextures.emplace_back( std::make_unique< renderer::StorageTextureBinding >( layoutBinding
			, view
			, index ) );
		return *m_storageTextures.back();
	}

	renderer::UniformBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_uniformBuffers.emplace_back( std::make_unique< renderer::UniformBufferBinding >( layoutBinding
			, uniformBuffer
			, offset
			, range
			, index ) );
		return *m_uniformBuffers.back();
	}

	renderer::StorageBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_storageBuffers.emplace_back( std::make_unique< renderer::StorageBufferBinding >( layoutBinding
			, storageBuffer
			, offset
			, range
			, index ) );
		return *m_storageBuffers.back();
	}

	renderer::TexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & buffer
		, renderer::BufferView const & view
		, uint32_t index )
	{
		m_texelBuffers.emplace_back( std::make_unique< renderer::TexelBufferBinding >( layoutBinding
			, buffer
			, view
			, index ) );
		return *m_texelBuffers.back();
	}

	renderer::DynamicUniformBufferBinding const & DescriptorSet::createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_dynamicUniformBuffers.emplace_back( std::make_unique< renderer::DynamicUniformBufferBinding >( layoutBinding
			, uniformBuffer
			, offset
			, range
			, index ) );
		m_dynamicBuffers.emplace_back( *m_dynamicUniformBuffers.back() );
		return *m_dynamicUniformBuffers.back();
	}

	renderer::DynamicStorageBufferBinding const & DescriptorSet::createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_dynamicStorageBuffers.emplace_back( std::make_unique< renderer::DynamicStorageBufferBinding >( layoutBinding
			, storageBuffer
			, offset
			, range
			, index ) );
		m_dynamicBuffers.emplace_back( *m_dynamicStorageBuffers.back() );
		return *m_dynamicStorageBuffers.back();
	}

	void DescriptorSet::update()const
	{
		std::sort( m_dynamicBuffers.begin()
			, m_dynamicBuffers.end()
			, []( std::reference_wrapper< renderer::DescriptorSetBinding > const & lhs
				, std::reference_wrapper< renderer::DescriptorSetBinding > const & rhs )
		{
			return lhs.get().getBinding().getBindingPoint() < rhs.get().getBinding().getBindingPoint();
		} );
	}
}
