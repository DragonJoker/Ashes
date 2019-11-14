/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/UniformBuffer.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	UniformBuffer::UniformBuffer( Device const & device
		, VkDeviceSize count
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: m_device{ device }
		, m_count{ count }
		, m_size{ size }
		, m_buffer{ std::make_unique< BufferBase >( device
			, count * getAlignedSize( getElementSize() )
			, usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
			, std::move( sharingMode ) ) }
	{
	}

	void UniformBuffer::bindMemory( DeviceMemoryPtr memory )
	{
		m_buffer->bindMemory( std::move( memory ) );
	}

	VkMemoryRequirements UniformBuffer::getMemoryRequirements()const
	{
		return m_buffer->getMemoryRequirements();
	}

	VkDeviceSize UniformBuffer::getAlignedSize( VkDeviceSize size )const
	{
		return ashes::getAlignedSize( size
			, m_device.getProperties().limits.minUniformBufferOffsetAlignment );
	}
}
