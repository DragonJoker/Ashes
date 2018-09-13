/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlNextSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>

namespace gl_renderer
{
	NextSubpassCommand::NextSubpassCommand( Device const & device
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
	{
	}

	void NextSubpassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "NextSubpassCommand" );
		if ( m_frameBuffer.getFrameBuffer() )
		{
			m_frameBuffer.setDrawBuffers( context, m_subpass.colorAttachments );
		}
	}

	CommandPtr NextSubpassCommand::clone()const
	{
		return std::make_unique< NextSubpassCommand >( *this );
	}
}
