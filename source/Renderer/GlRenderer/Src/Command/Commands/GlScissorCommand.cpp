/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	ScissorCommand::ScissorCommand( Device const & device
		, renderer::Scissor const & scissor )
		: m_device{ device }
		, m_scissor{ scissor }
	{
	}

	void ScissorCommand::apply()const
	{
		auto & save = m_device.getCurrentScissor();

		if ( m_scissor != save )
		{
			glLogCommand( "ScissorCommand" );
			glLogCall( gl::Scissor
				, m_scissor.offset.x
				, m_scissor.offset.y
				, m_scissor.size.width
				, m_scissor.size.height );
			save = m_scissor;
		}
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
