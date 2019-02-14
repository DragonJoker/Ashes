/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDispatchIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

namespace gl_renderer
{
	DispatchIndirectCommand::DispatchIndirectCommand( Device const & device
		, ashes::BufferBase const & buffer
		, uint32_t offset )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
	{
	}

	void DispatchIndirectCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DispatchIndirectCommand" );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DISPATCH_INDIRECT
			, m_buffer.getBuffer() );
		glLogCall( context
			, glDispatchComputeIndirect
			, GLintptr( BufferOffset( m_offset ) ) );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DISPATCH_INDIRECT, 0 );
	}

	CommandPtr DispatchIndirectCommand::clone()const
	{
		return std::make_unique< DispatchIndirectCommand >( *this );
	}
}
