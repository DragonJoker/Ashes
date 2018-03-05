#include "Descriptor/VkDescriptorSet.hpp"

#include "Buffer/VkBuffer.hpp"
#include "Buffer/VkBufferView.hpp"
#include "Buffer/VkUniformBuffer.hpp"
#include "Descriptor/VkDescriptorSetBinding.hpp"
#include "Descriptor/VkDescriptorSetLayout.hpp"
#include "Descriptor/VkDescriptorSetLayoutBinding.hpp"
#include "Descriptor/VkDescriptorPool.hpp"
#include "Image/VkSampler.hpp"
#include "Image/VkTextureView.hpp"

namespace vk_renderer
{
	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorPool const & pool
		, DescriptorSetLayout const & layout
		, uint32_t bindingPoint )
		: renderer::DescriptorSet{ pool, bindingPoint }
		, m_device{ device }
		, m_pool{ pool }
		, m_layout{ layout }
	{
		VkDescriptorSetAllocateInfo allocateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,           // sType
			nullptr,                                                  // pNext
			pool,                                                     // descriptorPool
			1u,                                                       // descriptorSetCount
			&static_cast< VkDescriptorSetLayout const & >( m_layout ) // pSetLayouts
		};
		DEBUG_DUMP( allocateInfo );
		auto res = m_device.vkAllocateDescriptorSets( m_device
			, &allocateInfo
			, &m_descriptorSet );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Descriptor set allocation failed: " + getLastError() };
		}
	}

	DescriptorSet::~DescriptorSet()
	{
		m_bindings.clear();

		if ( !m_pool.hasAutomaticFree() )
		{
			m_device.vkFreeDescriptorSets( m_device
				, m_pool
				, 1u
				, &m_descriptorSet );
		}
	}

	renderer::CombinedTextureSamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::Sampler const & sampler
		, renderer::ImageLayout layout
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< CombinedTextureSamplerBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view )
			, static_cast< Sampler const & >( sampler )
			, layout
			, index ) );
		return static_cast< CombinedTextureSamplerBinding const & >( *m_bindings.back() );
	}

	renderer::SamplerBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Sampler const & sampler
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< SamplerBinding >( layoutBinding
			, *this
			, static_cast< Sampler const & >( sampler )
			, index ) );
		return static_cast< SamplerBinding const & >( *m_bindings.back() );
	}

	renderer::SampledTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, renderer::ImageLayout layout
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< SampledTextureBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view )
			, layout
			, index ) );
		return static_cast< SampledTextureBinding const & >( *m_bindings.back() );
	}

	renderer::StorageTextureBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::TextureView const & view
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< StorageTextureBinding >( layoutBinding
			, *this
			, static_cast< TextureView const & >( view )
			, index ) );
		return static_cast< StorageTextureBinding const & >( *m_bindings.back() );
	}

	renderer::UniformBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< UniformBufferBinding >( layoutBinding
			, *this
			, static_cast< UniformBuffer const & >( uniformBuffer )
			, offset
			, range
			, index ) );
		return static_cast< UniformBufferBinding const & >( *m_bindings.back() );
	}

	renderer::StorageBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< StorageBufferBinding >( layoutBinding
			, *this
			, static_cast< Buffer const & >( storageBuffer )
			, offset
			, range
			, index ) );
		return static_cast< StorageBufferBinding const & >( *m_bindings.back() );
	}

	renderer::TexelBufferBinding const & DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & buffer
		, renderer::BufferView const & view
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< TexelBufferBinding >( layoutBinding
			, *this
			, static_cast< Buffer const & >( buffer )
			, static_cast< BufferView const & >( view )
			, index ) );
		return static_cast< TexelBufferBinding const & >( *m_bindings.back() );
	}

	renderer::DynamicUniformBufferBinding const & DescriptorSet::createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< DynamicUniformBufferBinding >( layoutBinding
			, *this
			, static_cast< UniformBuffer const & >( uniformBuffer )
			, offset
			, range
			, index ) );
		return static_cast< DynamicUniformBufferBinding const & >( *m_bindings.back() );
	}

	renderer::DynamicStorageBufferBinding const & DescriptorSet::createDynamicBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::BufferBase const & storageBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bindings.emplace_back( std::make_unique< DynamicStorageBufferBinding >( layoutBinding
			, *this
			, static_cast< Buffer const & >( storageBuffer )
			, offset
			, range
			, index ) );
		return static_cast< DynamicStorageBufferBinding const & >( *m_bindings.back() );
	}

	void DescriptorSet::update()const
	{
		auto bindings = makeVkArray < VkWriteDescriptorSet >( m_bindings );
		DEBUG_DUMP( bindings );
		m_device.vkUpdateDescriptorSets( m_device
			, static_cast< uint32_t >( bindings.size() )
			, bindings.data()
			, 0
			, nullptr );
	}
}
