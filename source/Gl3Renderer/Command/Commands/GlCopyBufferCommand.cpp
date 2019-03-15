/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include <Ashes/Miscellaneous/BufferCopy.hpp>

namespace gl_renderer
{
	CopyBufferCommand::CopyBufferCommand( Device const & device
		, ashes::BufferCopy const & copyInfo
		, ashes::BufferBase const & src
		, ashes::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
	{
	}

	void CopyBufferCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyBufferCommand" );
		if ( m_src.getTarget() == m_dst.getTarget() )
		{
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_READ
				, m_src.getBuffer() );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE
				, m_dst.getBuffer() );
			glLogCall( context
				, glCopyBufferSubData
				, GL_BUFFER_TARGET_COPY_READ
				, GL_BUFFER_TARGET_COPY_WRITE
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE
				, 0u );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_READ
				, 0u );
		}
		else
		{
			glLogCall( context
				, glBindBuffer
				, m_src.getTarget()
				, m_src.getBuffer() );
			glLogCall( context
				, glBindBuffer
				, m_dst.getTarget()
				, m_dst.getBuffer() );
			glLogCall( context
				, glCopyBufferSubData
				, m_src.getTarget()
				, m_dst.getTarget()
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( context
				, glBindBuffer
				, m_dst.getTarget()
				, 0u );
			glLogCall( context
				, glBindBuffer
				, m_src.getTarget()
				, 0u );
		}
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
