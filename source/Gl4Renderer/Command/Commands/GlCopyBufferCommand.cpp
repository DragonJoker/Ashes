/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	CopyBufferCommand::CopyBufferCommand( VkDevice device
		, VkBufferCopy copyInfo
		, VkBuffer src
		, VkBuffer dst )
		: CommandBase{ device }
		, m_src{ static_cast< VkBuffer >( src ) }
		, m_dst{ static_cast< VkBuffer >( dst ) }
		, m_copyInfo{ std::move( copyInfo ) }
	{
	}

	void CopyBufferCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyBufferCommand" );

		if ( get( m_src )->getTarget() == get( m_dst )->getTarget() )
		{
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_READ
				, get( m_src )->getInternal() );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE
				, get( m_dst )->getInternal() );
			glLogCall( context
				, glCopyBufferSubData
				, GL_BUFFER_TARGET_COPY_READ
				, GL_BUFFER_TARGET_COPY_WRITE
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_WRITE, 0u );
			glLogCall( context
				, glBindBuffer
				, GL_BUFFER_TARGET_COPY_READ, 0u );
		}
		else
		{
			glLogCall( context
				, glBindBuffer
				, get( m_src )->getTarget()
				, get( m_src )->getInternal() );
			glLogCall( context
				, glBindBuffer
				, get( m_dst )->getTarget()
				, get( m_dst )->getInternal() );
			glLogCall( context
				, glCopyBufferSubData
				, get( m_src )->getTarget()
				, get( m_dst )->getTarget()
				, m_copyInfo.srcOffset
				, m_copyInfo.dstOffset
				, m_copyInfo.size );
			glLogCall( context
				, glBindBuffer
				, get( m_dst )->getTarget()
				, 0u );
			glLogCall( context
				, glBindBuffer
				, get( m_src )->getTarget()
				, 0u );
		}
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
