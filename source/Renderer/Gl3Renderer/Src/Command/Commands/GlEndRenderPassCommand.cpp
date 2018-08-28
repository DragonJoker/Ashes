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

	void EndRenderPassCommand::apply()const
	{
		glLogCommand( "EndRenderPassCommand" );
		glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, 0u );
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}
