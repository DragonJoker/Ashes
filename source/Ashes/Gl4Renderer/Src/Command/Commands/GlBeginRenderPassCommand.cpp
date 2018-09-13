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
		void doClear( ContextLock const & context
			, ashes::AttachmentDescription const & attach
			, ashes::ClearValue const & clearValue
			, GLint & colourIndex
			, GLint & depthStencilIndex )
		{
			if ( clearValue.isColour() )
			{
				auto & colour = clearValue.colour();
				glLogCall( context
					, glClearBufferfv
					, GL_CLEAR_TARGET_COLOR
					, colourIndex
					, colour.float32.data() );
				++colourIndex;
			}
			else
			{
				glLogCall( context
					, glDepthMask
					, GL_TRUE );
				auto & depthStencil = clearValue.depthStencil();
				auto stencil = GLint( depthStencil.stencil );

				if ( ashes::isDepthStencilFormat( attach.format ) )
				{
					glLogCall( context
						, glClearBufferfi
						, GL_CLEAR_TARGET_DEPTH_STENCIL
						, 0u
						, depthStencil.depth
						, stencil );
				}
				else if ( ashes::isDepthFormat( attach.format ) )
				{
					glLogCall( context
						, glClearBufferfv
						, GL_CLEAR_TARGET_DEPTH
						, 0u
						, &depthStencil.depth );
				}
				else if ( ashes::isStencilFormat( attach.format ) )
				{
					glLogCall( context
						, glClearBufferiv
						, GL_CLEAR_TARGET_STENCIL
						, 0u
						, &stencil );
				}

				++depthStencilIndex;
			}
		}

		GLbitfield doClearBack( ContextLock const & context
			, ashes::AttachmentDescription const & attach
			, ashes::ClearValue const & clearValue
			, GLint & colourIndex
			, GLint & depthStencilIndex )
		{
			GLbitfield result{ 0u };

			if ( clearValue.isColour() )
			{
				auto & colour = clearValue.colour();
				glLogCall( context
					, glClearColor
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

				if ( ashes::isDepthStencilFormat( attach.format ) )
				{
					glLogCall( context
						, glClearDepth
						, depthStencil.depth );
					glLogCall( context
						, glClearStencil
						, depthStencil.stencil );
					result = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				}
				else if ( ashes::isDepthFormat( attach.format ) )
				{
					glLogCall( context
						, glClearDepth
						, depthStencil.depth );
					result = GL_DEPTH_BUFFER_BIT;
				}
				else if ( ashes::isStencilFormat( attach.format ) )
				{
					glLogCall( context
						, glClearStencil
						, depthStencil.stencil );
					result = GL_STENCIL_BUFFER_BIT;
				}

				++depthStencilIndex;
			}

			return result;
		}
	}

	BeginRenderPassCommand::BeginRenderPassCommand( Device const & device
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::ClearValueArray const & clearValues
		, ashes::SubpassContents contents
		, ashes::SubpassDescription const & subpass )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_subpass{ subpass }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_clearValues{ clearValues }
		, m_scissor{ 0, 0, m_frameBuffer.getDimensions().width, m_frameBuffer.getDimensions().height }
	{
	}

	void BeginRenderPassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BeginRenderPassCommand" );

		GLint colourIndex = 0u;
		GLint depthStencilIndex = 0u;
		auto & save = m_device.getCurrentScissor();

		if ( save != m_scissor )
		{
			glLogCall( context
				, glScissor
				, m_scissor.offset.x
				, m_scissor.offset.y
				, m_scissor.size.width
				, m_scissor.size.height );
		}

		if ( m_frameBuffer.isSRGB() )
		{
			glLogCall( context
				, glEnable
				, GL_FRAMEBUFFER_SRGB );
		}
		else
		{
			glLogCall( context
				, glDisable
				, GL_FRAMEBUFFER_SRGB );
		}

		if ( m_frameBuffer.getFrameBuffer()
			&& ( m_subpass.colorAttachments.size() != 1
				|| ( m_subpass.colorAttachments.size() == 1 && m_frameBuffer.getAllAttaches()[m_subpass.colorAttachments[0].attachment].object != GL_INVALID_INDEX ) ) )
		{
			assert( ( m_frameBuffer.getFrameBuffer() && ( m_frameBuffer.getSize() - m_subpass.resolveAttachments.size() ) == m_clearValues.size() )
				|| !m_frameBuffer.getFrameBuffer() );
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, m_frameBuffer.getFrameBuffer() );
			m_frameBuffer.setDrawBuffers( context
				, m_renderPass.getColourAttaches() );
			auto it = m_frameBuffer.begin();

			for ( size_t i = 0; i < m_frameBuffer.getSize() && i < m_clearValues.size(); ++i )
			{
				auto & clearValue = m_clearValues[i];
				auto & attach = *it;
				++it;

				if ( attach.getAttachment().loadOp == ashes::AttachmentLoadOp::eClear )
				{
					doClear( context
						, attach.getAttachment()
						, clearValue
						, colourIndex 
						, depthStencilIndex );
				}
			}

			m_frameBuffer.setDrawBuffers( context
				, m_subpass.colorAttachments );
		}
		else if ( m_frameBuffer.getFrameBuffer()
			&& m_subpass.colorAttachments.size() == 1
			&& m_frameBuffer.getAllAttaches()[m_subpass.colorAttachments[0].attachment].object == GL_INVALID_INDEX )
		{
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, 0 );
			auto & subAttach = m_subpass.colorAttachments[0];
			auto & attach = *( m_renderPass.getAttachments().begin() + subAttach.attachment );

			if ( attach.loadOp == ashes::AttachmentLoadOp::eClear )
			{
				auto & clearValue = m_clearValues[subAttach.attachment];
				auto bitfield = doClearBack( context
					, attach
					, clearValue
					, colourIndex
					, depthStencilIndex );
				glLogCall( context
					, glClear
					, bitfield );
			}
		}
		else if ( !m_clearValues.empty() )
		{
			assert( ( m_frameBuffer.getFrameBuffer() && ( m_frameBuffer.getSize() - m_subpass.resolveAttachments.size() ) == m_clearValues.size() )
				|| !m_frameBuffer.getFrameBuffer() );
			glLogCall( context
				, glBindFramebuffer
				, GL_FRAMEBUFFER
				, 0 );
			GLbitfield bitfield{ 0u };
			auto it = m_renderPass.getAttachments().begin();

			for ( size_t i = 0; i < m_renderPass.getAttachmentCount() && i < m_clearValues.size(); ++i )
			{
				auto & attach = *it;
				++it;

				if ( attach.loadOp == ashes::AttachmentLoadOp::eClear )
				{
					auto & clearValue = m_clearValues[i];
					bitfield |= doClearBack( context
						, attach
						, clearValue
						, colourIndex
						, depthStencilIndex );
				}
			}

			glLogCall( context
				, glClear
				, bitfield );
		}

		if ( save != m_scissor )
		{
			glLogCall( context
				, glScissor
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
