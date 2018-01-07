#include "VkDescriptorSet.hpp"

#include "VkBuffer.hpp"
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
		, m_layout{ pool.getLayout() }
	{
		auto layouts = makeVkArray< VkDescriptorSetLayout >( DescriptorSetLayoutCRefArray{ pool.getLayout() } );
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
