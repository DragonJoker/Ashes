/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlSetDepthBiasCommand.hpp"

namespace gl_renderer
{
	SetDepthBiasCommand::SetDepthBiasCommand( Device const & device
		, float constantFactor
		, float clamp
		, float slopeFactor )
		: CommandBase{ device }
		, m_constantFactor{ constantFactor }
		, m_clamp{ clamp }
		, m_slopeFactor{ slopeFactor }
	{
	}

	void SetDepthBiasCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "SetDepthBiasCommand" );
		glLogCall( context
			, glPolygonOffsetClampEXT
			, m_slopeFactor
			, m_constantFactor
			, m_clamp );
	}

	CommandPtr SetDepthBiasCommand::clone()const
	{
		return std::make_unique< SetDepthBiasCommand >( *this );
	}
}
