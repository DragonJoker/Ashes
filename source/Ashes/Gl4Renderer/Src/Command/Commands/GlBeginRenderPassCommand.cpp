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
		GLbitfield doClearColour( ContextLock const & context
			, ashes::ClearValue const & clearValue
			, uint32_t colourIndex )
		{
			assert( clearValue.isColour() );
			auto & colour = clearValue.colour();
			glLogCall( context
				, glClearBufferfv
				, GL_CLEAR_TARGET_COLOR
				, colourIndex
				, colour.float32.data() );
			return 0;
		}

		GLbitfield doClearBackColour( ContextLock const & context
			, ashes::ClearValue const & clearValue
			, uint32_t colourIndex )
		{
			assert( clearValue.isColour() );
			auto & colour = clearValue.colour();
			glLogCall( context
				, glClearColor
				, colour.float32[0]
				, colour.float32[1]
				, colour.float32[2]
				, colour.float32[3] );
			return GLbitfield( GL_COLOR_BUFFER_BIT );
		}

		GLbitfield doClearDepthStencil( ContextLock const & context
			, ashes::AttachmentDescription const & attach
			, ashes::ClearValue const & clearValue )
		{
			assert( !clearValue.isColour() );
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

			return 0;
		}

		GLbitfield doClearBackDepthStencil( ContextLock const & context
			, ashes::AttachmentDescription const & attach
			, ashes::ClearValue const & clearValue )
		{
			assert( !clearValue.isColour() );
			GLbitfield result{ 0u };
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

			return result;
		}
	}

	BeginRenderPassCommand::BeginRenderPassCommand( Device const & device
		, ashes::RenderPass const & renderPass
		, ashes::FrameBuffer const & frameBuffer
		, ashes::ClearValueArray const & clearValues
		, ashes::SubpassContents contents )
		: CommandBase{ device }
		, m_renderPass{ static_cast< RenderPass const & >( renderPass ) }
		, m_frameBuffer{ static_cast< FrameBuffer const & >( frameBuffer ) }
		, m_scissor{ 0, 0, m_frameBuffer.getDimensions().width, m_frameBuffer.getDimensions().height }
	{
		for ( auto & clearValue : clearValues )
		{
			if ( clearValue.isColour() )
			{
				m_rtClearValues.push_back( clearValue );
			}
			else
			{
				m_dsClearValue = clearValue;
			}
		}
	}

	void BeginRenderPassCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BeginRenderPassCommand" );

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

		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_frameBuffer.getFrameBuffer() );
		auto clearColour = &doClearColour;
		auto clearDepthStencil = &doClearDepthStencil;

		if ( m_frameBuffer.getFrameBuffer() )
		{
			m_frameBuffer.setDrawBuffers( context
				, m_renderPass.getColourAttaches() );
		}
		else
		{
			clearColour = &doClearBackColour;
			clearDepthStencil = &doClearBackDepthStencil;
		}

		auto & attaches = m_frameBuffer.getAllAttaches();
		uint32_t clearIndex = 0u;
		GLbitfield mask = 0u;

		for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
		{
			auto & attach = attaches[viewIndex];
			auto & attachDesc = m_renderPass.getAttachments()[viewIndex];

			if ( attachDesc.loadOp == ashes::AttachmentLoadOp::eClear )
			{
				if ( getAspectMask( attachDesc.format ) == ashes::ImageAspectFlag::eColour )
				{
					mask |= clearColour( context
						, m_rtClearValues[clearIndex]
						, clearIndex );
					++clearIndex;
				}
				else
				{
					mask |= clearDepthStencil( context
						, attachDesc
						, m_dsClearValue );
				}
			}
		}

		if ( !m_frameBuffer.getFrameBuffer() )
		{
			glLogCall( context
				, glClear
				, mask );
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

		m_device.setCurrentFramebuffer( m_frameBuffer.getFrameBuffer() );
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
