/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "RenderPass/GlRenderSubpass.hpp"

#include "RenderPass/GlFrameBuffer.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	RenderSubpass::RenderSubpass( renderer::Device const & device
		, renderer::PipelineBindPoint pipelineBindPoint
		, renderer::RenderSubpassState const & state
		, renderer::RenderSubpassAttachmentArray const & inputAttaches
		, renderer::RenderSubpassAttachmentArray const & colourAttaches
		, renderer::RenderSubpassAttachmentArray const & resolveAttaches
		, renderer::RenderSubpassAttachment const * depthAttach
		, renderer::UInt32Array const & preserveAttaches )
		: renderer::RenderSubpass{ device, pipelineBindPoint, state, inputAttaches, colourAttaches, resolveAttaches, depthAttach, preserveAttaches }
		, m_attaches{ colourAttaches }
		, m_resolveAttaches{ resolveAttaches }
	{
	}

	void RenderSubpass::resolveAttachments( FrameBuffer const & fbo )const
	{
		uint32_t index = 0u;

		for ( auto & resolveAttach : m_resolveAttaches )
		{
			auto & srcattach = fbo.getColourAttaches()[index++];
			auto & dstattach = fbo.getColourAttaches()[resolveAttach.attachment];

			if ( dstattach.object != GL_INVALID_INDEX )
			{
				gl::BindFramebuffer( GL_DRAW_FRAMEBUFFER, fbo.getFrameBuffer() );
			}
			else
			{
				gl::BindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
			}

			GLenum attach[1]{ dstattach.point };
			gl::BindFramebuffer( GL_READ_FRAMEBUFFER, fbo.getFrameBuffer() );
			gl::ReadBuffer( srcattach.point );
			gl::DrawBuffers( 1u, attach );
			gl::BlitFramebuffer( 0, 0, fbo.getDimensions().width, fbo.getDimensions().height, 0, 0, fbo.getDimensions().width, fbo.getDimensions().height, GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST );
		}
	}
}
