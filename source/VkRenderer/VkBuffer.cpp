#include "VkBuffer.hpp"

#include "VkDevice.hpp"
#include "VkBufferMemoryBarrier.hpp"

namespace vk_renderer
{
	BufferBase::BufferBase( renderer::Device const & device
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::BufferBase{ device
			, size
			, target
			, flags }
		, m_buffer{ static_cast< Device const & >( device ).getDevice()
			, size
			, convert( target )
			, convert( flags ) }
	{
	}

	uint8_t * BufferBase::lock( uint32_t offset
		, uint32_t size
		, renderer::MemoryMapFlags flags )const
	{
		return m_buffer.lock( offset, size, flags );
	}

	void BufferBase::unlock( uint32_t size
		, bool modified )const
	{
		m_buffer.unlock( size, modified );
	}

	renderer::BufferMemoryBarrier BufferBase::makeTransferDestination()const
	{
		return convert( *this, m_buffer.makeTransferDestination() );
	}

	renderer::BufferMemoryBarrier BufferBase::makeTransferSource()const
	{
		return convert( *this, m_buffer.makeTransferSource() );
	}

	renderer::BufferMemoryBarrier BufferBase::makeVertexShaderInputResource()const
	{
		return convert( *this, m_buffer.makeVertexShaderInputResource() );
	}

	renderer::BufferMemoryBarrier BufferBase::makeUniformBufferInput()const
	{
		return convert( *this, m_buffer.makeUniformBufferInput() );
	}

	renderer::BufferMemoryBarrier BufferBase::makeMemoryTransitionBarrier( renderer::AccessFlags dstAccess )const
	{
		return convert( *this, m_buffer.makeMemoryTransitionBarrier( convert( dstAccess ) ) );
	}
}
