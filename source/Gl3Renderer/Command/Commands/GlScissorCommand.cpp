/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	ScissorCommand::ScissorCommand( Device const & device
		, uint32_t firstScissor
		, ashes::ScissorArray const & scissors )
		: CommandBase{ device }
		, m_scissors{ scissors.begin() + firstScissor, scissors.end() }
	{
	}

	void ScissorCommand::apply( ContextLock const & context )const
	{
		auto & save = m_device.getCurrentScissor();
		auto & scissor = *m_scissors.begin();

		if ( scissor != save )
		{
			glLogCommand( "ScissorCommand" );
			glLogCall( context
				, glScissor
				, scissor.offset.x
				, scissor.offset.y
				, scissor.size.width
				, scissor.size.height );
			save = scissor;
		}
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
