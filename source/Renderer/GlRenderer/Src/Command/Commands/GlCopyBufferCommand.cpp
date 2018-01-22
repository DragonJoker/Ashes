/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include <Miscellaneous/BufferCopy.hpp>

namespace gl_renderer
{
	CopyBufferCommand::CopyBufferCommand( renderer::BufferCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::BufferBase const & dst )
		: m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
	{
	}

	void CopyBufferCommand::apply()const
	{
		glLogCall( glBindBuffer, GL_COPY_READ_BUFFER, m_src.getBuffer() );
		glLogCall( glBindBuffer, GL_COPY_WRITE_BUFFER, m_dst.getBuffer() );
		glLogCall( glCopyBufferSubData
			, GL_COPY_READ_BUFFER
			, GL_COPY_WRITE_BUFFER
			, m_copyInfo.srcOffset
			, m_copyInfo.dstOffset
			, m_copyInfo.size );
		glLogCall( glBindBuffer, GL_COPY_WRITE_BUFFER, 0u );
		glLogCall( glBindBuffer, GL_COPY_READ_BUFFER, 0u );
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
