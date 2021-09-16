/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndRenderPassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildEndRenderPassCommand( ContextStateStack & stack
		, CmdList & list )
	{
		if ( stack.hasCurrentFramebuffer() )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );
			stack.setCurrentFramebuffer( nullptr );
		}
	}
}
