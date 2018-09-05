/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlSetLineWidthCommand.hpp"

namespace gl_renderer
{
	SetLineWidthCommand::SetLineWidthCommand( Device const & device
		, float width )
		: CommandBase{ device }
		, m_width{ width }
	{
	}

	void SetLineWidthCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "SetLineWidthCommand" );
		glLogCall( context
			, glLineWidth
			, m_width );
	}

	CommandPtr SetLineWidthCommand::clone()const
	{
		return std::make_unique< SetLineWidthCommand >( *this );
	}
}
