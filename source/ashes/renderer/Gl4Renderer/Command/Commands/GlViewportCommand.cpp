/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list )
	{
		glLogCommand( "ViewportCommand" );
		stack.apply( list, firstViewport, viewports, false );
	}
}
