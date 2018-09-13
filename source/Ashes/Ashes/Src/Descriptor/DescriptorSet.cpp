/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorSet.hpp"

#include "Buffer/UniformBuffer.hpp"
#include "Core/Device.hpp"
#include "Descriptor/DescriptorSetLayoutBinding.hpp"
#include "Descriptor/DescriptorSetPool.hpp"

namespace ashes
{
	DescriptorSet::DescriptorSet( DescriptorPool const & pool, uint32_t bindingPoint )
		: m_device{ pool.getDevice() }
		, m_bindingPoint{ bindingPoint }
		, m_pool{ pool }
	{
		registerObject( m_device, "DescriptorSet", this );
	}

	DescriptorSet::~DescriptorSet()
	{
		unregisterObject( m_device, this );
	}

	void DescriptorSet::setBindings( WriteDescriptorSetArray bindings )
	{
		m_writes = std::move( bindings );
	}

	void DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, TextureView const & view
		, Sampler const & sampler
		, ImageLayout layout
		, uint32_t index )
	{
		m_writes.push_back( 
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eCombinedImageSampler,
			{ DescriptorImageInfo{ std::ref( sampler ), std::ref( view ), layout } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::Sampler const & sampler
		, uint32_t index )
	{
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eSampler,
			{ DescriptorImageInfo{ std::ref( sampler ), std::nullopt, ImageLayout::eShaderReadOnlyOptimal } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::TextureView const & view
		, ashes::ImageLayout layout
		, uint32_t index )
	{
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eSampledImage,
			{ DescriptorImageInfo{ std::nullopt, std::ref( view ), layout } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::TextureView const & view
		, uint32_t index )
	{
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eStorageImage,
			{ DescriptorImageInfo{ std::nullopt, std::ref( view ), ImageLayout::eGeneral } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eUniformBuffer,
			{},
			{ DescriptorBufferInfo{ uniformBuffer.getBuffer(), offset, range } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		auto type = checkFlag( buffer.getTargets(), ashes::BufferTarget::eStorageBuffer )
			? DescriptorType::eStorageBuffer
			: DescriptorType::eUniformBuffer;
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			type,
			{},
			{ DescriptorBufferInfo{ buffer, offset, range } },
		} );
	}

	void DescriptorSet::createBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::BufferBase const & buffer
		, ashes::BufferView const & view
		, uint32_t index )
	{
		auto type = checkFlag( buffer.getTargets(), ashes::BufferTarget::eStorageBuffer )
			? DescriptorType::eStorageTexelBuffer
			: DescriptorType::eUniformTexelBuffer;
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			type,
			{},
			{ DescriptorBufferInfo{ buffer, view.getOffset(), view.getRange() } },
			{ view },
		} );
	}

	void DescriptorSet::createDynamicBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::UniformBufferBase const & uniformBuffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			DescriptorType::eUniformBufferDynamic,
			{},
			{ DescriptorBufferInfo{ uniformBuffer.getBuffer(), offset, range } },
		} );
	}

	void DescriptorSet::createDynamicBinding( ashes::DescriptorSetLayoutBinding const & layoutBinding
		, ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t range
		, uint32_t index )
	{
		auto type = checkFlag( buffer.getTargets(), ashes::BufferTarget::eStorageBuffer )
			? DescriptorType::eStorageBufferDynamic
			: DescriptorType::eUniformBufferDynamic;
		m_writes.push_back(
		{
			layoutBinding.getBindingPoint(),
			index,
			1u,
			type,
			{},
			{ DescriptorBufferInfo{ buffer, offset, range } },
		} );
	}
}
