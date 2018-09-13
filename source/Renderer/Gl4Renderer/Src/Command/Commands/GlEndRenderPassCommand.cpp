/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlEndRenderPassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	EndRenderPassCommand::EndRenderPassCommand( Device const & device )
		: CommandBase{ device }
	{
	}

	void EndRenderPassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "EndRenderPassCommand" );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0u );
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}
