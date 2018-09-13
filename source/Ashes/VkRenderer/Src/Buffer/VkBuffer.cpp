#include "Buffer/VkBuffer.hpp"

#include "Core/VkDevice.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Sync/VkBufferMemoryBarrier.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, ashes::BufferTargets target )
		: ashes::BufferBase{ device
			, size
			, target }
		, m_device{ device }
	{
		VkBufferCreateInfo bufferCreate
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0,                                                // flags
			size,                                             // size
			convert( target ),                                // usage
			VK_SHARING_MODE_EXCLUSIVE,                        // sharingMode
			0,                                                // queueFamilyIndexCount
			nullptr                                           // pQueueFamilyIndices
		};
		DEBUG_DUMP( bufferCreate );
		auto res = m_device.vkCreateBuffer( m_device
			, &bufferCreate
			, nullptr
			, &m_buffer );
		checkError( res,  "Buffer creation" );
	}

	Buffer::~Buffer()
	{
		m_device.vkDestroyBuffer( m_device, m_buffer, nullptr );
	}

	ashes::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		return m_device.getBufferMemoryRequirements( m_buffer );
	}

	void Buffer::doBindMemory()
	{
		auto res = m_device.vkBindBufferMemory( m_device
			, m_buffer
			, static_cast< DeviceMemory const & >( *m_storage )
			, 0 );
		checkError( res, "Buffer memory binding" );
	}
}
