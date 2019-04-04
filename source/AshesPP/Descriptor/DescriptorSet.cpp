/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Descriptor/DescriptorSet.hpp"

#include "AshesPP/Buffer/Buffer.hpp"
#include "AshesPP/Buffer/BufferView.hpp"
#include "AshesPP/Buffer/UniformBuffer.hpp"
#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Descriptor/DescriptorPool.hpp"
#include "AshesPP/Descriptor/DescriptorSetLayout.hpp"
#include "AshesPP/Image/ImageView.hpp"
#include "AshesPP/Image/Sampler.hpp"

namespace ashes
{
	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorPool const & pool
		, DescriptorSetLayout const & layout
		, uint32_t bindingPoint )
		: m_device{ pool.getDevice() }
		, m_bindingPoint{ bindingPoint }
		, m_pool{ pool }
	{
		VkDescriptorSetLayout vkLayout = layout;
		VkDescriptorSetAllocateInfo allocateInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			nullptr,
			pool,
			1u,
			&vkLayout
		};
		DEBUG_DUMP( allocateInfo );
		auto res = m_device.vkAllocateDescriptorSets( m_device
			, &allocateInfo
			, &m_internal );
		checkError( res, "DescriptorSet allocation" );
		registerObject( m_device, "DescriptorSet", this );
	}

	DescriptorSet::~DescriptorSet()
	{
		unregisterObject( m_device, this );

		if ( !m_pool.hasAutomaticFree() )
		{
			m_device.vkFreeDescriptorSets( m_device
				, m_pool
				, 1u
				, &m_internal );
		}
	}

	void DescriptorSet::update()const
	{
		DEBUG_DUMP( m_writes );
		m_device.vkUpdateDescriptorSets( m_device
			, static_cast< uint32_t >( m_writes.size() )
			, m_writes.data()
			, 0
			, nullptr );
	}

	void DescriptorSet::setBindings( WriteDescriptorSetArray bindings )
	{
		m_writes = std::move( bindings );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, ImageView const & view
		, Sampler const & sampler
		, VkImageLayout layout
		, uint32_t index )
	{
		m_imageBindings.push_back( VkDescriptorImageInfo{ sampler, view, layout } );
		m_writes.push_back( VkWriteDescriptorSet
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				&m_imageBindings.back(),
				nullptr,
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, Sampler const & sampler
		, uint32_t index )
	{
		m_imageBindings.push_back( VkDescriptorImageInfo{ sampler, VK_NULL_HANDLE, VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER,
				&m_imageBindings.back(),
				nullptr,
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, ImageView const & view
		, VkImageLayout layout
		, uint32_t index )
	{
		m_imageBindings.push_back( VkDescriptorImageInfo{ VK_NULL_HANDLE, view, layout } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
				&m_imageBindings.back(),
				nullptr,
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, ImageView const & view
		, uint32_t index )
	{
		m_imageBindings.push_back( VkDescriptorImageInfo{ VK_NULL_HANDLE, view, VkImageLayout::VK_IMAGE_LAYOUT_GENERAL } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
				&m_imageBindings.back(),
				nullptr,
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bufferBindings.push_back( VkDescriptorBufferInfo{ uniformBuffer.getBuffer(), offset, range } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				nullptr,
				&m_bufferBindings.back(),
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, BufferBase const & buffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		auto type = ( buffer.getTargets() & VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
			? VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
			: VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_bufferBindings.push_back( VkDescriptorBufferInfo{ buffer, offset, range } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				type,
				nullptr,
				&m_bufferBindings.back(),
				nullptr,
			} );
	}

	void DescriptorSet::createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, BufferBase const & buffer
		, BufferView const & view
		, uint32_t index )
	{
		auto type = ( buffer.getTargets() & VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
			? VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
			: VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_bufferBindings.push_back( VkDescriptorBufferInfo{ buffer, view.getOffset(), view.getRange() } );
		m_bufferViews.push_back( view );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				type,
				nullptr,
				&m_bufferBindings.back(),
				&m_bufferViews.back(),
			} );
	}

	void DescriptorSet::createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_bufferBindings.push_back( VkDescriptorBufferInfo{ uniformBuffer.getBuffer(), offset, range } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
				nullptr,
				&m_bufferBindings.back(),
				nullptr,
			} );
	}

	void DescriptorSet::createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
		, BufferBase const & buffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		auto type = ( buffer.getTargets() & VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT )
			? VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
			: VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		m_bufferBindings.push_back( VkDescriptorBufferInfo{ buffer, offset, range } );
		m_writes.push_back(
			{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				m_internal,
				layoutBinding.binding,
				index,
				1u,
				type,
				nullptr,
				&m_bufferBindings.back(),
				nullptr,
			} );
	}
}
