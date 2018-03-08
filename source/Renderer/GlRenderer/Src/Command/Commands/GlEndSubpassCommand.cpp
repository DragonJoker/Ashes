/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlEndSubpassCommand.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderSubpass.hpp"

#include <RenderPass/ClearValue.hpp>
#include <RenderPass/RenderPassAttachment.hpp>

namespace gl_renderer
{
	EndSubpassCommand::EndSubpassCommand( Device const & device
		, renderer::FrameBuffer const & frameBuffer
		, RenderSubpass const & subpass )
		: m_device{ device }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_subpass{ subpass }
	{
	}

	void EndSubpassCommand::apply()const
	{
		glLogCommand( "EndSubpassCommand" );
		if ( m_subpass.hasResolveAttaches() )
		{
			if ( m_frameBuffer.getFrameBuffer() )
			{
				m_subpass.resolveAttachments( m_frameBuffer );
			}
		}
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
