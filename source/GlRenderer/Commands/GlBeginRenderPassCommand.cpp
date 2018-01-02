/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBeginRenderPassCommand.hpp"

#include "GlRenderer/GlFrameBuffer.hpp"
#include "GlRenderer/GlRenderPass.hpp"

#include <Renderer/ClearValue.hpp>

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

			for ( auto & clearValue : m_clearValues )
			{
				if ( clearValue.isColour() )
				{
					auto & colour = clearValue.colour();
					glClearColor( colour.r, colour.g, colour.b, colour.a );
					mask |= GL_COLOR_BUFFER_BIT;
				}
				else
				{
					auto & depthStencil = clearValue.depthStencil();
					glClearDepth( depthStencil.depth );
					glClearStencil( int( depthStencil.stencil ) );
					mask |= GL_DEPTH_BUFFER_BIT;
					mask |= GL_STENCIL_BUFFER_BIT;
				}
			}

			glClear( mask );
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
