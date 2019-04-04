/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndRenderPassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	EndRenderPassCommand::EndRenderPassCommand( VkDevice device )
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
		get( m_device )->setCurrentFramebuffer( 0u );
	}

	CommandPtr EndRenderPassCommand::clone()const
	{
		return std::make_unique< EndRenderPassCommand >( *this );
	}
}
