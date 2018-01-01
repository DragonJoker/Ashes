#include "GlBuffer.hpp"

#include "GlDevice.hpp"

namespace gl_renderer
{
	BufferBase::BufferBase( renderer::Device const & device
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags flags )
		: renderer::BufferBase{ device
			, size
			, target
			, flags }
		, m_target{ convert( target ) }
	{
		glGenBuffers( 1, &m_buffer );
		glBufferData( m_target, size, nullptr, GL_DYNAMIC_DRAW );
	}

	BufferBase::~BufferBase()
	{
		glDeleteBuffers( 1, &m_buffer );
	}

	uint8_t * BufferBase::lock( uint32_t offset
		, uint32_t size
		, renderer::MemoryMapFlags const & flags )const
	{
		glBindBuffer( m_target, m_buffer );
		return reinterpret_cast< uint8_t * >( glMapBufferRange( m_target, offset, size, convert( flags ) ) );
	}

	void BufferBase::unlock( uint32_t size
		, bool modified )const
	{
		glUnmapBuffer( m_target );
		glBindBuffer( m_target, 0u );
	}
}
