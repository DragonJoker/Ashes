/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/Buffer.hpp"

#include "ashespp/Core/Device.hpp"

#include <common/Exception.hpp>

namespace ashes
{
	BufferBase::BufferBase( Device const & device
		, VkDeviceSize size
		, VkBufferUsageFlags target )
		: m_device{ device }
		, m_size{ size }
		, m_target{ target }
	{
		VkBufferCreateInfo bufferCreate
		{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0,                                                // flags
			size,                                             // size
			target,                                           // usage
			VK_SHARING_MODE_EXCLUSIVE,                        // sharingMode
			0,                                                // queueFamilyIndexCount
			nullptr                                           // pQueueFamilyIndices
		};
		DEBUG_DUMP( bufferCreate );
		auto res = m_device.vkCreateBuffer( m_device
			, &bufferCreate
			, nullptr
			, &m_internal );
		checkError( res, "Buffer creation" );
		registerObject( m_device, "Buffer", this );
	}

	BufferBase::~BufferBase()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyBuffer( m_device, m_internal, nullptr );
	}

	VkMemoryRequirements BufferBase::getMemoryRequirements()const
	{
		return m_device.getBufferMemoryRequirements( m_internal );
	}

	void BufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		auto res = m_device.vkBindBufferMemory( m_device
			, m_internal
			, static_cast< DeviceMemory const & >( *m_storage )
			, 0 );
		checkError( res, "Buffer memory binding" );
	}

	uint8_t * BufferBase::lock( uint64_t offset
		, uint64_t size
		, VkMemoryMapFlags flags )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->lock( offset, size, flags );
	}

	void BufferBase::invalidate( uint64_t offset
		, uint64_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->invalidate( offset, size );
	}

	void BufferBase::flush( uint64_t offset
		, uint64_t size )const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->flush( offset, size );
	}

	void BufferBase::unlock()const
	{
		assert( m_storage && "The resource is not bound to a device memory object." );
		return m_storage->unlock();
	}

	VkBufferMemoryBarrier BufferBase::makeTransferDestination( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_TRANSFER_WRITE_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeTransferSource( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_TRANSFER_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeVertexShaderInputResource( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeUniformBufferInput( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_UNIFORM_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeMemoryTransitionBarrier( VkAccessFlags dstAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		VkBufferMemoryBarrier result
		{
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			m_currentAccessFlags,
			dstAccessFlags,
			srcQueueFamily,
			dstQueueFamily,
			*this,
			0,
			WholeSize
		};
		m_currentAccessFlags = dstAccessFlags;

		if ( ( m_currentAccessFlags & VK_ACCESS_INDIRECT_COMMAND_READ_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_INDEX_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_SHADER_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_SHADER_WRITE_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_UNIFORM_READ_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_COLOR_ATTACHMENT_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_TRANSFER_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_TRANSFER_WRITE_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if ( ( m_currentAccessFlags & VK_ACCESS_HOST_READ_BIT )
			|| ( m_currentAccessFlags & VK_ACCESS_HOST_WRITE_BIT ) )
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_HOST_BIT;
		}
		else
		{
			m_compatibleStageFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}

		return result;
	}
}
