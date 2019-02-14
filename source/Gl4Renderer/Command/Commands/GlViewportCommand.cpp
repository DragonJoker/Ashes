/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	ViewportCommand::ViewportCommand( Device const & device
		, uint32_t firstViewport
		, ashes::ViewportArray const & viewports )
		: CommandBase{ device }
		, m_viewports{ viewports.begin() + firstViewport, viewports.end() }
	{
	}

	void ViewportCommand::apply( ContextLock const & context )const
	{
		auto & save = m_device.getCurrentViewport();
		auto & viewport = *m_viewports.begin();

		if ( viewport != save )
		{
			glLogCommand( "ViewportCommand" );
			glLogCall( context
				, glViewport
				, viewport.offset.x
				, viewport.offset.y
				, viewport.size.width
				, viewport.size.height );
			glLogCall( context
				, glDepthRange
				, viewport.minDepth
				, viewport.maxDepth );
			save = viewport;
		}
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
