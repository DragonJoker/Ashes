/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDispatchIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	DispatchIndirectCommand::DispatchIndirectCommand( VkDevice device
		, VkBuffer buffer
		, VkDeviceSize offset )
		: CommandBase{ device }
		, m_buffer{ static_cast< VkBuffer >( buffer ) }
		, m_offset{ offset }
	{
	}

	void DispatchIndirectCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "DispatchIndirectCommand" );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_DISPATCH_INDIRECT
			, get( m_buffer )->getInternal() );
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
