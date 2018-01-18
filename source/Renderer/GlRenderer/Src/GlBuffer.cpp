#include "GlBuffer.hpp"

#include "GlDevice.hpp"

#include <Sync/BufferMemoryBarrier.hpp>

namespace gl_renderer
{
	Buffer::Buffer( renderer::Device const & device
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::BufferBase{ device
			, size
			, target
			, flags }
		, m_target{ convert( target ) }
	{
		glGenBuffers( 1, &m_glName );
		glBindBuffer( m_target, m_glName );
		glBufferData( m_target, size, nullptr, GL_DYNAMIC_DRAW );
		glBindBuffer( m_target, 0u );
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers( 1, &m_glName );
	}

	uint8_t * Buffer::lock( uint32_t offset
		, uint32_t size
		, renderer::MemoryMapFlags flags )const
	{
		glBindBuffer( m_target, m_glName );
		return reinterpret_cast< uint8_t * >( glMapBufferRange( m_target, offset, size, convert( flags ) ) );
	}

	void Buffer::unlock( uint32_t size
		, bool modified )const
	{
		glUnmapBuffer( m_target );
		glBindBuffer( m_target, 0u );
	}

	renderer::BufferMemoryBarrier Buffer::makeTransferDestination()const
	{
		return renderer::BufferMemoryBarrier{ renderer::AccessFlag::eTransferWrite
			, renderer::AccessFlag::eTransferWrite
			, 0u
			, 0u
			, *this
			, 0u
			, getSize() };
	}

	renderer::BufferMemoryBarrier Buffer::makeTransferSource()const
	{
		return renderer::BufferMemoryBarrier{ renderer::AccessFlag::eTransferRead
			, renderer::AccessFlag::eTransferRead
			, 0u
			, 0u
			, *this
			, 0u
			, getSize() };
	}

	renderer::BufferMemoryBarrier Buffer::makeVertexShaderInputResource()const
	{
		return renderer::BufferMemoryBarrier{ renderer::AccessFlag::eShaderRead
			, renderer::AccessFlag::eShaderRead
			, 0u
			, 0u
			, *this
			, 0u
			, getSize() };
	}

	renderer::BufferMemoryBarrier Buffer::makeUniformBufferInput()const
	{
		return renderer::BufferMemoryBarrier{ renderer::AccessFlag::eUniformRead
			, renderer::AccessFlag::eUniformRead
			, 0u
			, 0u
			, *this
			, 0u
			, getSize() };
	}

	renderer::BufferMemoryBarrier Buffer::makeMemoryTransitionBarrier( renderer::AccessFlags dstAccess )const
	{
		return renderer::BufferMemoryBarrier{ dstAccess
			, dstAccess
			, 0u
			, 0u
			, *this
			, 0u
			, getSize() };
	}
}
