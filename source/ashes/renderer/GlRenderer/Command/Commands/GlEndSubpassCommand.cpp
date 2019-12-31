/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndSubpassCommand.hpp"
#include "Command/Commands/GlBlitImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildEndSubpassCommand( VkDevice device
		, ContextStateStack & stack
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		glLogCommand( "EndSubpassCommand" );
		stack.apply( list, preExecuteActions, 0u, VkScissorArray{}, true );
		stack.apply( list, preExecuteActions, 0u, VkViewportArray{}, true );

		if ( subpass.pResolveAttachments 
			&& get( frameBuffer )->getInternal() )
		{
			uint32_t index = 0u;

			for ( auto & resolveAttach : makeArrayView( subpass.pResolveAttachments, subpass.colorAttachmentCount ) )
			{
				auto & srcAttach = get( frameBuffer )->getMsColourAttaches()[index++];
				auto & dstAttach = get( frameBuffer )->getAllAttaches()[resolveAttach.attachment];

				if ( srcAttach.originalObject != dstAttach.originalObject
					|| srcAttach.originalMipLevel != dstAttach.originalMipLevel )
				{
					// Setup source FBO
					auto srcFbo = get( device )->getBlitSrcFbo();
					list.push_back( makeCmd< OpType::eInitFramebuffer >( &get( srcFbo )->getInternal() ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
						, srcFbo ) );
					list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
						, srcAttach.point
						, srcAttach.target
						, srcAttach.object
						, srcAttach.mipLevel ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
						, nullptr ) );

					// Setup dst FBO
					auto dstFbo = get( device )->getBlitDstFbo();
					list.push_back( makeCmd< OpType::eInitFramebuffer >( &get( dstFbo )->getInternal() ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
						, dstFbo ) );
					list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
						, dstAttach.point
						, dstAttach.target
						, dstAttach.object
						, dstAttach.mipLevel ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
						, nullptr ) );

					// Perform blit
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
						, get( device )->getBlitSrcFbo() ) );
					list.push_back( makeCmd< OpType::eReadBuffer >( uint32_t( srcAttach.point ) ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
						, get( device )->getBlitDstFbo() ) );
					list.push_back( makeCmd< OpType::eDrawBuffers >( uint32_t( dstAttach.point ) ) );
					list.push_back( makeCmd< OpType::eBlitFramebuffer >(
						0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
						0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
						GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST ) );

					// Unbind
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
						, nullptr ) );
					list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
						, nullptr ) );
				}
			}
		}
	}
}
