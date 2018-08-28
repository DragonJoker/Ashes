/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	ViewportCommand::ViewportCommand( Device const & device
		, renderer::Viewport const & viewport )
		: CommandBase{ device }
		, m_viewport{ viewport }
	{
	}

	void ViewportCommand::apply()const
	{
		auto & save = m_device.getCurrentViewport();

		if ( m_viewport != save )
		{
			glLogCommand( "ViewportCommand" );
			glLogCall( m_device.getContext(), glViewport
				, m_viewport.offset.x
				, m_viewport.offset.y
				, m_viewport.size.width
				, m_viewport.size.height );
			glLogCall( m_device.getContext(), glDepthRange
				, m_viewport.minDepth
				, m_viewport.maxDepth );
			save = m_viewport;
		}
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
