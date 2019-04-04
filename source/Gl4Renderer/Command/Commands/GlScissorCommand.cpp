/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	ScissorCommand::ScissorCommand( VkDevice device
		, uint32_t firstScissor
		, VkScissorArray scissors )
		: CommandBase{ device }
		, m_scissors{ scissors.begin() + firstScissor, scissors.end() }
	{
	}

	void ScissorCommand::apply( ContextLock const & context )const
	{
		auto & save = get( m_device )->getCurrentScissor();
		auto & scissor = *m_scissors.begin();

		if ( scissor != save )
		{
			glLogCommand( "ScissorCommand" );
			glLogCall( context
				, glScissor
				, scissor.offset.x
				, scissor.offset.y
				, scissor.extent.width
				, scissor.extent.height );
			save = scissor;
		}
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
