/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

namespace gl_renderer
{
	CopyBufferCommand::CopyBufferCommand( renderer::BufferBase const & src
		, renderer::BufferBase const & dst
		, uint32_t size
		, uint32_t offset )
		: m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_size{ size }
		, m_offset{ offset }
	{
	}

	void CopyBufferCommand::apply()const
	{
		glLogCall( glBindBuffer, m_src.getTarget(), m_src.getBuffer() );
		glLogCall( glBindBuffer, m_dst.getTarget(), m_dst.getBuffer() );
		glLogCall( glCopyBufferSubData
			, m_src.getTarget()
			, m_dst.getTarget()
			, 0u
			, m_offset
			, m_size );
		glLogCall( glBindBuffer, m_src.getTarget(), 0u );
		glLogCall( glBindBuffer, m_dst.getTarget(), 0u );
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
