/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndRenderPassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void buildEndRenderPassCommand( ContextStateStack & stack
		, CmdList & list )
	{
		if ( stack.hasCurrentFramebuffer() )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );
			stack.setCurrentFramebuffer( VK_NULL_HANDLE );
		}
	}
}
