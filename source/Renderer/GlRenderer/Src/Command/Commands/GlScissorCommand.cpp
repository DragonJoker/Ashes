/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlScissorCommand.hpp"

namespace gl_renderer
{
	ScissorCommand::ScissorCommand( renderer::Scissor const & scissor )
		: m_scissor{ scissor }
	{
	}

	void ScissorCommand::apply()const
	{
		glLogCall( gl::Scissor
			, m_scissor.getOffset().x
			, m_scissor.getOffset().y
			, m_scissor.getSize().x
			, m_scissor.getSize().y );
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
