/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	DrawIndirectCommand::DrawIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode )
		: CommandBase{ device }
		, m_buffer{ static_cast< VkBuffer >( buffer ) }
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
			, get( m_buffer )->getInternal() );
		glLogCall( context
			, glMultiDrawArraysIndirect
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
