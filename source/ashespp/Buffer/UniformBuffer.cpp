/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/UniformBuffer.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	UniformBufferBase::UniformBufferBase( Device const & device
		, VkDeviceSize count
		, VkDeviceSize size
		, VkBufferUsageFlags target )
		: m_device{ device }
		, m_count{ count }
		, m_size{ size }
		, m_buffer{ m_device.createBuffer( count * getAlignedSize( getElementSize() )
			, target | VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT ) }
	{
	}

	UniformBufferBase::~UniformBufferBase()
	{
	}

	void UniformBufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		m_buffer->bindMemory( std::move( memory ) );
	}

	VkMemoryRequirements UniformBufferBase::getMemoryRequirements()const
	{
		return m_buffer->getMemoryRequirements();
	}

	VkDeviceSize UniformBufferBase::getAlignedSize( VkDeviceSize size )const
	{
		return ashes::getAlignedSize( size
			, m_device.getProperties().limits.minUniformBufferOffsetAlignment );
	}
}
