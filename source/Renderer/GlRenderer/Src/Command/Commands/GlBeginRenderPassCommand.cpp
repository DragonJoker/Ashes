/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBeginRenderPassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <RenderPass/ClearValue.hpp>

namespace gl_renderer
{
	BeginRenderPassCommand::BeginRenderPassCommand( renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents )
		: m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_clearValues{ clearValues }
	{
	}

	void BeginRenderPassCommand::apply()const
	{
		glBindFramebuffer( GL_FRAMEBUFFER, m_frameBuffer.getFrameBuffer() );

		if ( m_renderPass.getClear() )
		{
			GLbitfield mask = 0u;
			GLint colourIndex = 0u;
			GLint depthStencilIndex = 0u;

			for ( auto & clearValue : m_clearValues )
			{
				if ( clearValue.isColour() )
				{
					auto & colour = clearValue.colour();
					glClearBufferfv( GL_COLOR, colourIndex, colour.data );
					mask |= GL_COLOR_BUFFER_BIT;
					++colourIndex;
				}
				else
				{
					auto & depthStencil = clearValue.depthStencil();
					auto & attach = m_frameBuffer.getDepthStencilAttaches()[depthStencilIndex];

					switch ( attach.type )
					{
					case GL_DEPTH:
						glClearBufferfv( GL_DEPTH, 0u, &depthStencil.depth );
						mask |= GL_DEPTH_BUFFER_BIT;
						break;

					case GL_STENCIL:
						glClearBufferuiv( GL_STENCIL, 0u, &depthStencil.stencil );
						mask |= GL_STENCIL_BUFFER_BIT;
						break;

					case GL_DEPTH_STENCIL:
						glClearBufferfi( GL_DEPTH_STENCIL, 0u, depthStencil.depth, depthStencil.stencil );
						mask |= GL_DEPTH_BUFFER_BIT;
						mask |= GL_STENCIL_BUFFER_BIT;
						break;
					}

					++depthStencilIndex;
				}
			}
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
