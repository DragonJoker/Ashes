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
		auto & viewport = *m_viewports.begin();

		if ( viewport != context->getCurrentViewport() )
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
			context->setCurrentViewport( viewport );
		}
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
