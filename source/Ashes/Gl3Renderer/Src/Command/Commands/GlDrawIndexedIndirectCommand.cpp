/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlDrawIndexedIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	DrawIndexedIndirectCommand::DrawIndexedIndirectCommand( Device const & device
		, ashes::BufferBase const & buffer
		, uint32_t offset
		, uint32_t drawCount
		, uint32_t stride
		, ashes::PrimitiveTopology mode
		, ashes::IndexType type )
		: CommandBase{ device }
		, m_buffer{ static_cast< Buffer const & >( buffer ) }
		, m_offset{ offset }
		, m_drawCount{ drawCount }
		, m_stride{ stride }
		, m_mode{ convert( mode ) }
		, m_type{ convert( type ) }
	{
		if ( !m_device.getPhysicalDevice().getFeatures().multiDrawIndirect )
		{
			throw std::runtime_error( "Draw indirect is not supported" );
		}
	}

	void DrawIndexedIndirectCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DrawIndexedIndirectCommand" );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DRAW_INDIRECT
			, m_buffer.getBuffer() );
		glLogCall( context
			, glMultiDrawElementsIndirect_ARB
			, m_mode
			, m_type
			, BufferOffset( m_offset )
			, m_drawCount
			, m_stride );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DRAW_INDIRECT
			, 0 );
	}

	CommandPtr DrawIndexedIndirectCommand::clone()const
	{
		return std::make_unique< DrawIndexedIndirectCommand >( *this );
	}
}
