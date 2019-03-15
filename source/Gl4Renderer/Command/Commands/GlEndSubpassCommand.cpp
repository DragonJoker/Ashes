/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndSubpassCommand.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include <Ashes/RenderPass/ClearValue.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>
#include <Ashes/RenderPass/SubpassDescription.hpp>

namespace gl_renderer
{
	EndSubpassCommand::EndSubpassCommand( Device const & device
		, ashes::FrameBuffer const & frameBuffer
		, ashes::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_subpass{ subpass }
	{
		assert( m_subpass.resolveAttachments.empty()
			|| m_subpass.resolveAttachments.size() == m_subpass.colorAttachments.size() );
	}

	void EndSubpassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "EndSubpassCommand" );

		if ( !m_subpass.resolveAttachments.empty() )
		{
			if ( m_frameBuffer.getFrameBuffer() )
			{
				uint32_t index = 0u;

				for ( auto & resolveAttach : m_subpass.resolveAttachments )
				{
					auto & srcattach = m_frameBuffer.getColourAttaches()[index++];
					auto & dstattach = m_frameBuffer.getColourAttaches()[resolveAttach.attachment];

					if ( dstattach.object != GL_INVALID_INDEX )
					{
						context->glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_frameBuffer.getFrameBuffer() );
					}
					else
					{
						context->glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
					}

					GLenum attach[1]{ dstattach.point };
					context->glBindFramebuffer( GL_READ_FRAMEBUFFER, m_frameBuffer.getFrameBuffer() );
					context->glReadBuffer( srcattach.point );
					context->glDrawBuffers( 1u, attach );
					context->glBlitFramebuffer( 0, 0, m_frameBuffer.getDimensions().width, m_frameBuffer.getDimensions().height
						, 0, 0, m_frameBuffer.getDimensions().width, m_frameBuffer.getDimensions().height
						, GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST );
				}
			}
		}
	}

	CommandPtr EndSubpassCommand::clone()const
	{
		return std::make_unique< EndSubpassCommand >( *this );
	}
}
