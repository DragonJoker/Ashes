/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	ViewportCommand::ViewportCommand( VkDevice device
		, uint32_t firstViewport
		, VkViewportArray viewports )
		: CommandBase{ device }
		, m_viewports{ viewports.begin() + firstViewport, viewports.end() }
	{
	}

	void ViewportCommand::apply( ContextLock const & context )const
	{
		auto & save = get( m_device )->getCurrentViewport();
		auto & viewport = *m_viewports.begin();

		if ( viewport != save )
		{
			glLogCommand( "ViewportCommand" );
			glLogCall( context
				, glViewport
				, GLint( viewport.x )
				, GLint( viewport.y )
				, GLsizei( viewport.width )
				, GLsizei( viewport.height ) );
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
