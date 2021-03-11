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

	GLbitfield clearAttaches( VkFramebuffer framebuffer
		, VkRenderPass renderPass
		, VkClearValueArray rtClearValues
		, VkClearValue dsClearValue
		, CmdList & list
		, bool back )
	{
		uint32_t clearIndex = 0u;
		auto & attaches = get( framebuffer )->getRenderableAttaches();
		GLbitfield mask{ 0u };

		for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
		{
			if ( auto attachDesc = get( renderPass )->findAttachment( viewIndex ) )
			{
				if ( attachDesc->loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
				{
					if ( getAspectMask( attachDesc->format ) == VK_IMAGE_ASPECT_COLOR_BIT )
					{
						if ( back )
						{
							list.push_back( makeCmd< OpType::eClearBackColour >( rtClearValues[clearIndex].color
								, clearIndex ) );
						}
						else
						{
							list.push_back( makeCmd< OpType::eClearColour >( rtClearValues[clearIndex].color
								, clearIndex ) );
						}
						mask |= GL_COLOR_BUFFER_BIT;
						++clearIndex;
					}
					else
					{
						if ( isDepthStencilFormat( attachDesc->format ) )
						{
							if ( back )
							{
								list.push_back( makeCmd< OpType::eClearBackDepthStencil >( dsClearValue.depthStencil.depth
									, int32_t( dsClearValue.depthStencil.stencil ) ) );
							}
							else
							{
								list.push_back( makeCmd< OpType::eClearDepthStencil >( dsClearValue.depthStencil ) );
							}
							mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
						}
						else if ( isDepthFormat( attachDesc->format ) )
						{
							if ( back )
							{
								list.push_back( makeCmd< OpType::eClearBackDepth >( dsClearValue.depthStencil.depth ) );
							}
							else
							{
								list.push_back( makeCmd< OpType::eClearDepth >( dsClearValue.depthStencil.depth ) );
							}
							mask |= GL_DEPTH_BUFFER_BIT;
						}
						else if ( isStencilFormat( attachDesc->format ) )
						{
							if ( back )
							{
								list.push_back( makeCmd< OpType::eClearBackStencil >( int32_t( dsClearValue.depthStencil.stencil ) ) );
							}
							else
							{
								list.push_back( makeCmd< OpType::eClearStencil >( int32_t( dsClearValue.depthStencil.stencil ) ) );
							}
							mask |= GL_STENCIL_BUFFER_BIT;
						}
					}
				}
			}
		}

		return mask;
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

		if ( get( renderPass )->getMaxLoadClearIndex() != InvalidIndex )
		{
			assert( clearValues.size() >= get( renderPass )->getMaxLoadClearIndex() );

			for ( auto & reference : get( renderPass )->getFboAttachable() )
			{
				auto & attach = get( renderPass )->getAttachment( reference );
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
			list.insert( list.end()
				, get( frameBuffer )->getBindAttaches().begin()
				, get( frameBuffer )->getBindAttaches().end() );
			auto references = makeArrayView( get( renderPass )->getFboAttachable().begin()
				, get( renderPass )->getFboAttachable().end() );
			list.push_back( makeCmd< OpType::eDrawBuffers >( get( frameBuffer )->getDrawBuffers( references ) ) );
			clearAttaches( frameBuffer, renderPass, rtClearValues, dsClearValue, list, false );
		}
	}
}
