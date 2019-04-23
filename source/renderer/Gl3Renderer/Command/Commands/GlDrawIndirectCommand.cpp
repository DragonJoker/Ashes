/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

namespace gl_renderer
{
	DrawIndirectCommand::DrawIndirectCommand( Device const & device
		, ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_mode{ convert( mode ) }
	{
	}

	void DrawIndirectCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DrawIndirectCommand" );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DRAW_INDIRECT
			, m_buffer.getBuffer() );
		glLogCall( context
			, glMultiDrawArraysIndirect_ARB
			, m_mode
			, BufferOffset( m_offset )
			, m_drawCount
			, m_stride );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DRAW_INDIRECT
			, 0 );
	}

	CommandPtr DrawIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndirectCommand >( *this );
	}
}
