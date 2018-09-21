/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/AttachmentDescription.hpp>

namespace gl_renderer
{
	BeginSubpassCommand::BeginSubpassCommand( Device const & device
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
	{
	}

	void BeginSubpassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "NextSubpassCommand" );
		if ( m_frameBuffer.getFrameBuffer() )
		{
			m_frameBuffer.setDrawBuffers( context, m_subpass.colorAttachments );
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
