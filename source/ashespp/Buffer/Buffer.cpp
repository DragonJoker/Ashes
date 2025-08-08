/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Buffer/Buffer.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Miscellaneous/DeviceMemory.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	BufferBase::BufferBase( Device const & device
		, std::string const & debugName
		, VkBufferCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( bufferCreate );
		auto res = m_device.vkCreateBuffer( m_device
			, &m_createInfo
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Buffer creation" );
		registerObject( m_device, debugName, *this );
	}

	BufferBase::BufferBase( Device const & device
		, VkBufferCreateInfo createInfo )
		: BufferBase{ device, "Buffer", std::move( createInfo ) }
	{
	}

	BufferBase::BufferBase( Device const & device
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare const & sharingMode )
		: BufferBase{ device, "Buffer", size, usage, sharingMode }
	{
	}

	BufferBase::BufferBase( Device const & device
		, std::string const & debugName
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare const & sharingMode )
		: BufferBase{ device, debugName, makeCreateInfo( size, usage, sharingMode ) }
	{
	}

	BufferBase::BufferBase( Device const & device
		, VkBuffer buffer
		, VkDeviceMemory storage )
		: BufferBase{ device
			, buffer
			, storage
			, { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr, 0u
				, 0u
				, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
				, VK_SHARING_MODE_EXCLUSIVE
				, 0u, {} } }
	{
	}

	BufferBase::BufferBase( Device const & device
		, VkBuffer buffer
		, VkDeviceMemory storage
		, VkBufferCreateInfo createInfo )
		: BufferBase{ device, "Buffer", buffer, storage, std::move( createInfo ) }
	{
	}

	BufferBase::BufferBase( Device const & device
		, std::string const & debugName
		, VkBuffer buffer
		, VkDeviceMemory storage
		, VkBufferCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_internal{ buffer }
		, m_storage{ std::make_unique< DeviceMemory >( device, debugName, storage ) }
		, m_ownInternal{ false }
	{
	}

	BufferBase::~BufferBase()noexcept
	{
		if ( m_ownInternal )
		{
			unregisterObject( m_device, *this );
			m_device.vkDestroyBuffer( m_device
				, m_internal
				, m_device.getAllocationCallbacks() );
		}
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

#if VK_KHR_buffer_device_address

	VkDeviceAddress BufferBase::getDeviceAddress()const
	{
		VkBufferDeviceAddressInfoKHR info{ VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR
			, nullptr
			, m_internal };
		return m_device.vkGetBufferDeviceAddressKHR( m_device
			, &info );
	}

	uint64_t BufferBase::getOpaqueCaptureAddress()const
	{
		VkBufferDeviceAddressInfoKHR info{ VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR
			, nullptr
			, m_internal };
		return m_device.vkGetBufferOpaqueCaptureAddressKHR( m_device
			, &info );
	}

#endif

	VkPipelineStageFlags BufferBase::getCompatibleStageFlags()const
	{
		VkPipelineStageFlags result = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		if ( checkFlag( m_currentAccessFlags, VK_ACCESS_INDIRECT_COMMAND_READ_BIT ) )
		{
			result = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_INDEX_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT ) )
		{
			result = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT ) )
		{
			result = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_SHADER_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_SHADER_WRITE_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_UNIFORM_READ_BIT ) )
		{
			result = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) )
		{
			result = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT ) )
		{
			result = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_TRANSFER_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_TRANSFER_WRITE_BIT ) )
		{
			result = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if ( checkFlag( m_currentAccessFlags, VK_ACCESS_HOST_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_HOST_WRITE_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_MEMORY_READ_BIT )
			|| checkFlag( m_currentAccessFlags, VK_ACCESS_MEMORY_WRITE_BIT ) )
		{
			result = VK_PIPELINE_STAGE_HOST_BIT;
		}

		return result;
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

	VkBufferMemoryBarrier BufferBase::makeHostRead( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_HOST_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeHostWrite( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( VK_ACCESS_HOST_WRITE_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkBufferMemoryBarrier BufferBase::makeMemoryTransitionBarrier( VkAccessFlags dstAccessFlags
		, VkPipelineStageFlags dstStageFlags
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
		m_compatibleStageFlags = dstStageFlags;
		return result;
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
		m_compatibleStageFlags = getCompatibleStageFlags();
		return result;
	}
}
