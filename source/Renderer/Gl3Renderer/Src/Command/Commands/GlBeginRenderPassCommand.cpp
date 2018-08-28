/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBeginRenderPassCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include <RenderPass/ClearValue.hpp>

namespace gl_renderer
{
	namespace
	{
		void doClear( Device const & device
			, renderer::AttachmentDescription const & attach
			, renderer::ClearValue const & clearValue
			, GLint & colourIndex
			, GLint & depthStencilIndex )
		{
			if ( clearValue.isColour() )
			{
				auto & colour = clearValue.colour();
				glLogCall( device.getContext(), glClearBufferfv
					, GL_CLEAR_TARGET_COLOR
					, colourIndex
					, colour.float32.data() );
				++colourIndex;
			}
			else
			{
				glLogCall( device.getContext(), glDepthMask, GL_TRUE );
				auto & depthStencil = clearValue.depthStencil();
				auto stencil = GLint( depthStencil.stencil );
				auto format = getInternal( attach.format );

				if ( isDepthStencilFormat( format ) )
				{
					glLogCall( device.getContext(), glClearBufferfi, GL_CLEAR_TARGET_DEPTH_STENCIL, 0u, depthStencil.depth, stencil );
				}
				else if ( isDepthFormat( format ) )
				{
					glLogCall( device.getContext(), glClearBufferfv, GL_CLEAR_TARGET_DEPTH, 0u, &depthStencil.depth );
				}
				else if ( isStencilFormat( format ) )
				{
					glLogCall( device.getContext(), glClearBufferiv, GL_CLEAR_TARGET_STENCIL, 0u, &stencil );
				}

				++depthStencilIndex;
			}
		}

		GLbitfield doClearBack( Device const & device
			, renderer::AttachmentDescription const & attach
			, renderer::ClearValue const & clearValue
			, GLint & colourIndex
			, GLint & depthStencilIndex )
		{
			GLbitfield result{ 0u };

			if ( clearValue.isColour() )
			{
				auto & colour = clearValue.colour();
				glLogCall( device.getContext(), glClearColor
					, colour.float32[0]
					, colour.float32[1]
					, colour.float32[2]
					, colour.float32[3] );
				result = GL_COLOR_BUFFER_BIT;
				++colourIndex;
			}
			else
			{
				auto & depthStencil = clearValue.depthStencil();
				auto stencil = GLint( depthStencil.stencil );
				auto format = getInternal( attach.format );

				if ( isDepthStencilFormat( format ) )
				{
					glLogCall( device.getContext(), glClearDepth, depthStencil.depth );
					glLogCall( device.getContext(), glClearStencil, depthStencil.stencil );
					result = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				}
				else if ( isDepthFormat( format ) )
				{
					glLogCall( device.getContext(), glClearDepth, depthStencil.depth );
					result = GL_DEPTH_BUFFER_BIT;
				}
				else if ( isStencilFormat( format ) )
				{
					glLogCall( device.getContext(), glClearStencil, depthStencil.stencil );
					result = GL_STENCIL_BUFFER_BIT;
				}

				++depthStencilIndex;
			}

			return result;
		}
	}

	BeginRenderPassCommand::BeginRenderPassCommand( Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::FrameBuffer const & frameBuffer
		, renderer::ClearValueArray const & clearValues
		, renderer::SubpassContents contents
		, renderer::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_clearValues{ clearValues }
		, m_scissor{ 0, 0, m_frameBuffer.getDimensions().width, m_frameBuffer.getDimensions().height }
	{
	}

	void BeginRenderPassCommand::apply()const
	{
		glLogCommand( "BeginRenderPassCommand" );

		GLint colourIndex = 0u;
		GLint depthStencilIndex = 0u;
		auto & save = m_device.getCurrentScissor();

		if ( save != m_scissor )
		{
			glLogCall( m_device.getContext(), glScissor
				, m_scissor.offset.x
				, m_scissor.offset.y
				, m_scissor.size.width
				, m_scissor.size.height );
		}

		if ( m_frameBuffer.isSRGB() )
		{
			m_device.getContext().glEnable( GL_FRAMEBUFFER_SRGB );
		}
		else
		{
			m_device.getContext().glDisable( GL_FRAMEBUFFER_SRGB );
		}

		if ( m_frameBuffer.getFrameBuffer()
			&& ( m_subpass.colorAttachments.size() != 1
				|| ( m_subpass.colorAttachments.size() == 1 && m_frameBuffer.getAllAttaches()[m_subpass.colorAttachments[0].attachment].object != GL_INVALID_INDEX ) ) )
		{
			assert( ( m_frameBuffer.getFrameBuffer() && ( m_frameBuffer.getSize() - m_subpass.resolveAttachments.size() ) == m_clearValues.size() )
				|| !m_frameBuffer.getFrameBuffer() );
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, m_frameBuffer.getFrameBuffer() );
			m_frameBuffer.setDrawBuffers( m_renderPass.getColourAttaches() );
			auto it = m_frameBuffer.begin();

			for ( size_t i = 0; i < m_frameBuffer.getSize() && i < m_clearValues.size(); ++i )
			{
				auto & clearValue = m_clearValues[i];
				auto & attach = *it;
				++it;

				if ( attach.getAttachment().loadOp == renderer::AttachmentLoadOp::eClear )
				{
					doClear( m_device
						, attach.getAttachment()
						, clearValue
						, colourIndex 
						, depthStencilIndex );
				}
			}

			m_frameBuffer.setDrawBuffers( m_subpass.colorAttachments );
		}
		else if ( m_frameBuffer.getFrameBuffer()
			&& m_subpass.colorAttachments.size() == 1
			&& m_frameBuffer.getAllAttaches()[m_subpass.colorAttachments[0].attachment].object == GL_INVALID_INDEX )
		{
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, 0 );
			auto & subAttach = m_subpass.colorAttachments[0];
			auto & attach = *( m_renderPass.getAttachments().begin() + subAttach.attachment );

			if ( attach.loadOp == renderer::AttachmentLoadOp::eClear )
			{
				auto & clearValue = m_clearValues[subAttach.attachment];
				auto bitfield = doClearBack( m_device
					, attach
					, clearValue
					, colourIndex
					, depthStencilIndex );
				glLogCall( m_device.getContext(), glClear, bitfield );
			}
		}
		else if ( !m_clearValues.empty() )
		{
			assert( ( m_frameBuffer.getFrameBuffer() && ( m_frameBuffer.getSize() - m_subpass.resolveAttachments.size() ) == m_clearValues.size() )
				|| !m_frameBuffer.getFrameBuffer() );
			glLogCall( m_device.getContext(), glBindFramebuffer, GL_FRAMEBUFFER, 0 );
			GLbitfield bitfield{ 0u };
			auto it = m_renderPass.getAttachments().begin();

			for ( size_t i = 0; i < m_renderPass.getAttachmentCount() && i < m_clearValues.size(); ++i )
			{
				auto & attach = *it;
				++it;

				if ( attach.loadOp == renderer::AttachmentLoadOp::eClear )
				{
					auto & clearValue = m_clearValues[i];
					bitfield |= doClearBack( m_device
						, attach
						, clearValue
						, colourIndex
						, depthStencilIndex );
				}
			}

			glLogCall( m_device.getContext(), glClear, bitfield );
		}

		if ( save != m_scissor )
		{
			glLogCall( m_device.getContext(), glScissor
				, save.offset.x
				, save.offset.y
				, save.size.width
				, save.size.height );
		}
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
