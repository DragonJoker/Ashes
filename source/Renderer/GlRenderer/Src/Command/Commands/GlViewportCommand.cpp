/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlViewportCommand.hpp"

namespace gl_renderer
{
	ViewportCommand::ViewportCommand( renderer::Viewport const & viewport )
		: m_viewport{ viewport }
	{
	}

	void ViewportCommand::apply()const
	{
		glLogCall( glViewport
			, m_viewport.getOffset().x
			, m_viewport.getOffset().y
			, m_viewport.getSize().x
			, m_viewport.getSize().y );
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
