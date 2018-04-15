#include "Buffer/VkBuffer.hpp"

#include "Core/VkDevice.hpp"
#include "Miscellaneous/VkDeviceMemory.hpp"
#include "Sync/VkBufferMemoryBarrier.hpp"

#include <Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	Buffer::Buffer( Device const & device
		, uint32_t size
		, renderer::BufferTargets target )
		: renderer::BufferBase{ device
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

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Buffer creation failed: " + getLastError() };
		}
	}

	Buffer::~Buffer()
	{
		m_device.vkDestroyBuffer( m_device, m_buffer, nullptr );
	}

	renderer::MemoryRequirements Buffer::getMemoryRequirements()const
	{
		return m_device.getBufferMemoryRequirements( m_buffer );
	}

	void Buffer::doBindMemory()
	{
		auto res = m_device.vkBindBufferMemory( m_device
			, m_buffer
			, static_cast< DeviceMemory const & >( *m_storage )
			, 0 );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Buffer memory binding failed: " + getLastError() };
		}
	}
}
