/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <Ashes/RenderPass/AttachmentDescription.hpp>

namespace gl_renderer
{
	BeginSubpassCommand::BeginSubpassCommand( Device const & device
		, RenderPass const & renderPass
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

		if ( m_frameBuffer.getInternal() )
		{
			m_frameBuffer.setDrawBuffers( context
				, m_subpass.colorAttachments );
		}
	}

	CommandPtr BeginSubpassCommand::clone()const
	{
		return std::make_unique< BeginSubpassCommand >( *this );
	}
}
