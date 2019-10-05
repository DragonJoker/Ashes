/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginRenderPassCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl4_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
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
		auto & attaches = get( framebuffer )->getAllAttaches();
		GLbitfield mask{ 0u };

		for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
		{
			auto & attach = attaches[viewIndex];

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
		, CmdList & list )
	{
		VkClearValueArray rtClearValues{};
		VkClearValue dsClearValue{};
		assert( clearValues.size() == get( renderPass )->size() );
		auto index = 0u;

		for ( auto & clearValue : clearValues )
		{
			if ( auto attach = get( renderPass )->findAttachment( index ) )
			{
				if ( ashes::isDepthOrStencilFormat( attach->format ) )
				{
					dsClearValue = clearValue;
				}
				else
				{
					rtClearValues.push_back( clearValue );
				}
			}

			++index;
		}

		glLogCommand( "BeginRenderPassCommand" );
		VkScissorArray scissors
		{
			VkRect2D
			{
				{ 0, 0 },
				{ get( frameBuffer )->getWidth(), get( frameBuffer )->getHeight() }
			}
		};

		if ( stack.getCurrentScissors() != scissors )
		{
			list.push_back( makeCmd< OpType::eApplyScissors >( scissors ) );
			stack.setCurrentScissors( scissors );
		}

		if ( get( frameBuffer )->isSRGB() )
		{
			list.push_back( makeCmd< OpType::eEnable >( GL_FRAMEBUFFER_SRGB ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eDisable >( GL_FRAMEBUFFER_SRGB ) );
		}

		if ( !stack.hasCurrentFramebuffer()
			|| stack.getCurrentFramebuffer() != frameBuffer )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, frameBuffer ) );
			stack.setCurrentFramebuffer( frameBuffer );
		}

		uint32_t clearIndex = 0u;
		auto & attaches = get( frameBuffer )->getAllAttaches();
		UInt32Array drawBuffers;

		if ( get( frameBuffer )->getInternal() )
		{
			list.insert( list.end()
				, get( frameBuffer )->getBindAttaches().begin()
				, get( frameBuffer )->getBindAttaches().end() );
			auto & attachments = get( frameBuffer )->getAttachments();

			for ( auto & attach : get( renderPass )->getColourAttaches() )
			{
				auto fboAttach = attachments[attach.index];
				auto fboView = get( fboAttach );
				auto fboImage = get( fboView->getImage() );

				if ( fboImage->hasInternal() )
				{
					drawBuffers.push_back( getAttachmentPoint( attach.attach.get().format ) + attach.index );
				}
				else if ( attaches.size() == 1 )
				{
					drawBuffers.push_back( GL_ATTACHMENT_POINT_BACK );
				}
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( std::move( drawBuffers ) ) );
			clearAttaches( frameBuffer, renderPass, rtClearValues, dsClearValue, list, false );
		}
		else
		{
			auto mask = clearAttaches( frameBuffer, renderPass, rtClearValues, dsClearValue, list, true );
			list.push_back( makeCmd< OpType::eClearBack >( mask ) );
		}
	}
}
