/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/Buffer.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"
#include "Sync/BufferMemoryBarrier.hpp"

namespace ashes
{
	BufferBase::BufferBase( Device const & device
		, uint32_t size
		, BufferTargets target )
		: m_device{ device }
		, m_size{ size }
		, m_target{ target }
	{
		registerObject( m_device, "Buffer", this );
	}

	BufferBase::~BufferBase()
	{
		unregisterObject( m_device, this );
	}

	void BufferBase::bindMemory( DeviceMemoryPtr memory )
	{
		assert( !m_storage && "A resource can only be bound once to a device memory object." );
		m_storage = std::move( memory );
		doBindMemory();
	}

	uint8_t * BufferBase::lock( uint64_t offset
		, uint64_t size
		, MemoryMapFlags flags )const
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

	BufferMemoryBarrier BufferBase::makeTransferDestination( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( ashes::AccessFlag::eTransferWrite
			, srcQueueFamily
			, dstQueueFamily );
	}

	BufferMemoryBarrier BufferBase::makeTransferSource( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( ashes::AccessFlag::eTransferRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	BufferMemoryBarrier BufferBase::makeVertexShaderInputResource( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( ashes::AccessFlag::eVertexAttributeRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	BufferMemoryBarrier BufferBase::makeUniformBufferInput( uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeMemoryTransitionBarrier( ashes::AccessFlag::eUniformRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	BufferMemoryBarrier BufferBase::makeMemoryTransitionBarrier( AccessFlags dstAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		BufferMemoryBarrier result
		{
			m_currentAccessFlags,
			dstAccessFlags,
			srcQueueFamily,
			dstQueueFamily,
			*this,
			0,
			~( 0ull )
		};
		m_currentAccessFlags = dstAccessFlags;

		if ( checkFlag( m_currentAccessFlags, AccessFlag::eIndirectCommandRead ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eDrawIndirect;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eIndexRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eVertexAttributeRead ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eVertexInput;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eInputAttachmentRead ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eFragmentShader;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eShaderRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eShaderWrite )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eUniformRead ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eVertexShader;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eColourAttachmentRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eColourAttachmentWrite ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eColourAttachmentOutput;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eDepthStencilAttachmentRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eDepthStencilAttachmentWrite ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eEarlyFragmentTests;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eTransferRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eTransferWrite ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eTransfer;
		}
		else if ( checkFlag( m_currentAccessFlags, AccessFlag::eHostRead )
			|| checkFlag( m_currentAccessFlags, AccessFlag::eHostWrite ) )
		{
			m_compatibleStageFlags = PipelineStageFlag::eHost;
		}
		else
		{
			m_compatibleStageFlags = PipelineStageFlag::eBottomOfPipe;
		}

		return result;
	}
}
