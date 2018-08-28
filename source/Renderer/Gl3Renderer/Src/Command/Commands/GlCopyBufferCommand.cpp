/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include <Miscellaneous/BufferCopy.hpp>

namespace gl_renderer
{
	CopyBufferCommand::CopyBufferCommand( Device const & device
		, renderer::BufferCopy const & copyInfo
		, renderer::BufferBase const & src
		, renderer::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
	{
	}

	void CopyBufferCommand::apply()const
	{
		glLogCommand( "CopyBufferCommand" );
		if ( m_src.getTarget() == m_dst.getTarget() )
		{
			glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_READ, m_src.getBuffer() );
			glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_WRITE, m_dst.getBuffer() );
			glLogCall( m_device.getContext(), glCopyBufferSubData
				, GL_BUFFER_TARGET_COPY_READ
				, GL_BUFFER_TARGET_COPY_WRITE
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_WRITE, 0u );
			glLogCall( m_device.getContext(), glBindBuffer, GL_BUFFER_TARGET_COPY_READ, 0u );
		}
		else
		{
			glLogCall( m_device.getContext(), glBindBuffer, m_src.getTarget(), m_src.getBuffer() );
			glLogCall( m_device.getContext(), glBindBuffer, m_dst.getTarget(), m_dst.getBuffer() );
			glLogCall( m_device.getContext(), glCopyBufferSubData
				, m_src.getTarget()
				, m_dst.getTarget()
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( m_device.getContext(), glBindBuffer, m_dst.getTarget(), 0u );
			glLogCall( m_device.getContext(), glBindBuffer, m_src.getTarget(), 0u );
		}
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
