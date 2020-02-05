/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlViewportCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, ArrayView< VkViewport const > viewports
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		glLogCommand( list, "ViewportCommand" );
		stack.apply( list, preExecuteActions, firstViewport, viewports, false );
	}
}
