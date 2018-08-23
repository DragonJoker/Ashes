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

	void SetLineWidthCommand::apply()const
	{
		glLogCommand( "SetLineWidthCommand" );
		glLogCall( m_device.getContext()
			, glLineWidth
			, m_width );
	}

	CommandPtr SetLineWidthCommand::clone()const
	{
		return std::make_unique< SetLineWidthCommand >( *this );
	}
}
