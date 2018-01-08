#include "VkDescriptorSet.hpp"

#include "VkBuffer.hpp"
#include "VkDescriptorSetBinding.hpp"
#include "VkDescriptorSetLayoutBinding.hpp"
#include "VkDescriptorSetLayout.hpp"
#include "VkDescriptorSetPool.hpp"
#include "VkSampler.hpp"
#include "VkTexture.hpp"
#include "VkUniformBuffer.hpp"

namespace vk_renderer
{
	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorSetPool const & pool )
		: renderer::DescriptorSet{ pool }
		, m_device{ device }
		, m_pool{ pool }
		, m_layout{ static_cast< DescriptorSetLayout const & >( pool.getLayout() ) }
	{
		auto layouts = makeVkArray< VkDescriptorSetLayout >( DescriptorSetLayoutCRefArray{ m_layout } );
		VkDescriptorSetAllocateInfo allocateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, // sType
			nullptr,                                        // pNext
			pool,                                           // descriptorPool
			static_cast< uint32_t >( layouts.size() ),      // descriptorSetCount
			layouts.data()                                  // pSetLayouts
		};
		DEBUG_DUMP( allocateInfo );
		auto res = AllocateDescriptorSets( m_device
			, &allocateInfo
			, &m_descriptorSet );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Descriptor set allocation failed: " + getLastError() };
		}
	}

	DescriptorSet::~DescriptorSet()
	{
		if ( !m_pool.hasAutomaticFree() )
		{
			FreeDescriptorSets( m_device
				, m_pool
				, 1u
				, &m_descriptorSet );
		}
	}

	renderer::CombinedTextureSamplerBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view
		, renderer::Sampler const & sampler )
	{
		auto result = CombinedTextureSamplerBinding
		{
			layoutBinding,
			*this,
			static_cast< Texture const & >( view ),
			static_cast< Sampler const & >( sampler )
		};
		m_bindings.push_back( result );
		return result;
	}

	renderer::SampledTextureBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::Texture const & view )
	{
		auto result = SampledTextureBinding
		{
			layoutBinding,
			*this,
			static_cast< Texture const & >( view )
		};
		m_bindings.push_back( result );
		return result;
	}

	renderer::UniformBufferBinding DescriptorSet::createBinding( renderer::DescriptorSetLayoutBinding const & layoutBinding
		, renderer::UniformBufferBase const & uniformBuffer
		, uint32_t offset )
	{
		offset = uniformBuffer.getOffset( offset );
		auto result = UniformBufferBinding
		{
			layoutBinding,
			*this,
			uniformBuffer,
			offset
		};
		m_bindings.push_back( result );
		return result;
	}
	void DescriptorSet::update()const
	{
		DEBUG_DUMP( makeVkArray< VkWriteDescriptorSet >( m_bindings ) );
		UpdateDescriptorSets( m_device
			, static_cast< uint32_t >( m_bindings.size() )
			, m_bindings.data()
			, 0
			, nullptr );
	}
}
