/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndSubpassCommand.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	EndSubpassCommand::EndSubpassCommand( VkDevice device
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass )
		: CommandBase{ device }
		, m_frameBuffer{ frameBuffer }
		, m_subpass{ subpass }
	{
	}

	void EndSubpassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "EndSubpassCommand" );

		if ( m_subpass.pResolveAttachments )
		{
			if ( get( m_frameBuffer )->getInternal() )
			{
				uint32_t index = 0u;

				for ( auto it = m_subpass.pResolveAttachments;
					it != m_subpass.pResolveAttachments + m_subpass.colorAttachmentCount;
					++it )
				{
					auto & resolveAttach = *it;
					auto & srcattach = get( m_frameBuffer )->getColourAttaches()[index++];
					auto & dstattach = get( m_frameBuffer )->getColourAttaches()[resolveAttach.attachment];

					if ( dstattach.object != GL_INVALID_INDEX )
					{
						context->glBindFramebuffer( GL_DRAW_FRAMEBUFFER, get( m_frameBuffer )->getInternal() );
					}
					else
					{
						context->glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
					}

					GLenum attach[1]{ dstattach.point };
					context->glBindFramebuffer( GL_READ_FRAMEBUFFER, get( m_frameBuffer )->getInternal() );
					context->glReadBuffer( srcattach.point );
					context->glDrawBuffers( 1u, attach );
					context->glBlitFramebuffer( 0, 0, get( m_frameBuffer )->getWidth(), get( m_frameBuffer )->getHeight()
						, 0, 0, get( m_frameBuffer )->getWidth(), get( m_frameBuffer )->getHeight()
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
