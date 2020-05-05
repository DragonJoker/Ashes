/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/VertexBuffer.hpp"

namespace ashes
{
	VertexBufferBase::VertexBufferBase( Device const & device
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: VertexBufferBase{ device, "VertexBuffer", size, usage, std::move( sharingMode ) }
	{
	}
	
	VertexBufferBase::VertexBufferBase( Device const & device
		, std::string const & debugName
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode )
		: m_device{ device }
		, m_size{ size }
		, m_buffer{ m_device.createBuffer( debugName
			, size
			, usage | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
			, std::move( sharingMode ) ) }
	{
	}

	void VertexBufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		m_buffer->bindMemory( std::move( memory ) );
	}

	VkMemoryRequirements VertexBufferBase::getMemoryRequirements()const
	{
		return m_buffer->getMemoryRequirements();
	}
}
