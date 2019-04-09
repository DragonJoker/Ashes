/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginRenderPassCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		GLbitfield doClearColour( ContextLock const & context
			, VkClearValue const & clearValue
			, uint32_t colourIndex )
		{
			auto & colour = clearValue.color;
			glLogCall( context
				, glClearBufferfv
				, GL_CLEAR_TARGET_COLOR
				, colourIndex
				, colour.float32 );
			return 0;
		}

		GLbitfield doClearBackColour( ContextLock const & context
			, VkClearValue const & clearValue
			, uint32_t colourIndex )
		{
			auto & colour = clearValue.color;
			glLogCall( context
				, glClearColor
				, colour.float32[0]
				, colour.float32[1]
				, colour.float32[2]
				, colour.float32[3] );
			return GLbitfield( GL_COLOR_BUFFER_BIT );
		}

		GLbitfield doClearDepthStencil( ContextLock const & context
			, VkAttachmentDescription const & attach
			, VkClearValue const & clearValue )
		{
			glLogCall( context
				, glDepthMask
				, GL_TRUE );
			auto & depthStencil = clearValue.depthStencil;
			auto stencil = GLint( depthStencil.stencil );

			if ( isDepthStencilFormat( attach.format ) )
			{
				glLogCall( context
					, glClearBufferfi
					, GL_CLEAR_TARGET_DEPTH_STENCIL
					, 0u
					, depthStencil.depth
					, stencil );
			}
			else if ( isDepthFormat( attach.format ) )
			{
				glLogCall( context
					, glClearBufferfv
					, GL_CLEAR_TARGET_DEPTH
					, 0u
					, &depthStencil.depth );
			}
			else if ( isStencilFormat( attach.format ) )
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
			, VkAttachmentDescription const & attach
			, VkClearValue const & clearValue )
		{
			GLbitfield result{ 0u };
			auto & depthStencil = clearValue.depthStencil;
			auto stencil = GLint( depthStencil.stencil );

			if ( isDepthStencilFormat( attach.format ) )
			{
				glLogCall( context
					, glClearDepth
					, depthStencil.depth );
				glLogCall( context
					, glClearStencil
					, depthStencil.stencil );
				result = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
			}
			else if ( isDepthFormat( attach.format ) )
			{
				glLogCall( context
					, glClearDepth
					, depthStencil.depth );
				result = GL_DEPTH_BUFFER_BIT;
			}
			else if ( isStencilFormat( attach.format ) )
			{
				glLogCall( context
					, glClearStencil
					, depthStencil.stencil );
				result = GL_STENCIL_BUFFER_BIT;
			}

			return result;
		}
	}

	BeginRenderPassCommand::BeginRenderPassCommand( VkDevice device
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray clearValues
		, VkSubpassContents contents )
		: CommandBase{ device }
		, m_renderPass{ renderPass }
		, m_frameBuffer{ frameBuffer }
		, m_scissor{ 0, 0, get( m_frameBuffer )->getWidth(), get( m_frameBuffer )->getHeight() }
	{
		for ( auto & clearValue : clearValues )
		{
			if ( clearValue.color.int32[0] != 0
				|| clearValue.color.int32[1] != 0
				|| clearValue.color.int32[2] != 0
				|| clearValue.color.int32[3] != 0 )
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

		auto & save = context->getCurrentScissor();

		if ( save != m_scissor )
		{
			glLogCall( context
				, glScissor
				, m_scissor.offset.x
				, m_scissor.offset.y
				, m_scissor.extent.width
				, m_scissor.extent.height );
		}

		if ( get( m_frameBuffer )->isSRGB() )
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
			, get( m_frameBuffer )->getInternal() );
		auto clearColour = &doClearColour;
		auto clearDepthStencil = &doClearDepthStencil;

		if ( get( m_frameBuffer )->getInternal() )
		{
			get( m_frameBuffer )->setDrawBuffers( context
				, get( m_renderPass )->getColourAttaches() );
		}
		else
		{
			clearColour = &doClearBackColour;
			clearDepthStencil = &doClearBackDepthStencil;
		}

		auto & attaches = get( m_frameBuffer )->getAllAttaches();
		uint32_t clearIndex = 0u;
		GLbitfield mask = 0u;

		for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
		{
			auto & attach = attaches[viewIndex];
			auto & attachDesc = get( m_renderPass )->getAttachments()[viewIndex];

			if ( attachDesc.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
			{
				if ( getAspectMask( attachDesc.format ) == VK_IMAGE_ASPECT_COLOR_BIT )
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

		if ( !get( m_frameBuffer )->getInternal() )
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
				, save.extent.width
				, save.extent.height );
		}

		context->setCurrentFramebuffer( get( m_frameBuffer )->getInternal() );
	}

	CommandPtr BeginRenderPassCommand::clone()const
	{
		return std::make_unique< BeginRenderPassCommand >( *this );
	}
}
