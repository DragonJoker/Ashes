/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl3_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
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
