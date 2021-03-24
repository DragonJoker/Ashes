/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginRenderPassCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdClearBack const & cmd )
	{
		glLogCall( context
			, glClear
			, cmd.mask );
	}

	void apply( ContextLock const & context
		, CmdClearBackColour const & cmd )
	{
		glLogCall( context
			, glClearColor
			, cmd.color.float32[0]
			, cmd.color.float32[1]
			, cmd.color.float32[2]
			, cmd.color.float32[3] );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
	}

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd )
	{
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepthStencil const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void clearAttach( FboAttachment attach
		, uint32_t index
		, VkRenderPass renderPass
		, VkClearValueArray rtClearValues
		, VkClearValue dsClearValue
		, CmdList & list
		, uint32_t & clearIndex )
	{
		if ( auto attachDesc = get( renderPass )->findAttachment( index ) )
		{
			if ( attachDesc->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
			{
				if ( getAspectMask( attachDesc->format ) == VK_IMAGE_ASPECT_COLOR_BIT )
				{
					list.push_back( makeCmd< OpType::eClearColour >( rtClearValues[clearIndex].color, 0u ) );
					++clearIndex;
				}
				else
				{
					if ( isDepthStencilFormat( attachDesc->format ) )
					{
						list.push_back( makeCmd< OpType::eClearDepthStencil >( dsClearValue.depthStencil ) );
					}
					else if ( isDepthFormat( attachDesc->format ) )
					{
						list.push_back( makeCmd< OpType::eClearDepth >( dsClearValue.depthStencil.depth ) );
					}
					else if ( isStencilFormat( attachDesc->format ) )
					{
						list.push_back( makeCmd< OpType::eClearStencil >( int32_t( dsClearValue.depthStencil.stencil ) ) );
					}
				}
			}
		}
	}

	void buildBeginRenderPassCommand( ContextStateStack & stack
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray clearValues
		, VkSubpassContents contents
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		VkClearValueArray rtClearValues{};
		VkClearValue dsClearValue{};
		stack.applyDisableBlend( list );

		if ( get( renderPass )->getMaxLoadClearIndex() != InvalidIndex )
		{
			assert( clearValues.size() >= get( renderPass )->getMaxLoadClearIndex() );

			for ( auto & reference : get( renderPass )->getFboAttachable() )
			{
				auto & attach = get( renderPass )->getAttachment( reference );

				if ( attach.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR
					|| attach.stencilLoadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
				{
					auto & clearValue = clearValues[reference.attachment];

					if ( ashes::isDepthOrStencilFormat( attach.format ) )
					{
						dsClearValue = clearValue;
					}
					else
					{
						rtClearValues.push_back( clearValue );
					}
				}
			}
		}

		glLogCommand( list, "BeginRenderPassCommand" );
		stack.setRenderArea( get( frameBuffer )->getDimensions() );
		stack.apply( list, preExecuteActions, 0u, ArrayView< VkRect2D const >(), true );
		stack.apply( list, preExecuteActions, 0u, ArrayView< VkViewport const >(), true );
		stack.applySRGBStatus( list, get( frameBuffer )->isSRGB() );

		if ( ( !stack.hasCurrentFramebuffer()
			|| stack.getCurrentFramebuffer() != frameBuffer )
			&& get( frameBuffer )->getInternal() != GL_INVALID_INDEX )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, frameBuffer ) );
			stack.setCurrentFramebuffer( frameBuffer );
		}

		if ( get( frameBuffer )->getInternal() != GL_INVALID_INDEX )
		{
			assert( get( frameBuffer )->getInternal() );
			uint32_t clearIndex = 0u;

			for ( auto & reference : get( renderPass )->getFboAttachable() )
			{
				auto attach = get( frameBuffer )->getAttachment( reference );

				if ( attach.point )
				{
					attach.bindDraw( stack, 0u, GL_FRAMEBUFFER, list );
					clearAttach( attach, reference.attachment, renderPass, rtClearValues, dsClearValue, list, clearIndex );
				}
			}
		}
	}
}
