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
	void buildEndSubpassCommand( [[maybe_unused]] VkDevice device
		, ContextStateStack & stack
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		glLogCommand( list, "EndSubpassCommand" );
		stack.apply( list, preExecuteActions, 0u, ArrayView< VkRect2D const >(), true );
		stack.apply( list, preExecuteActions, 0u, ArrayView< VkViewport const >(), true );

		if ( subpass.pResolveAttachments 
			&& get( frameBuffer )->getInternal()
			&& get( frameBuffer )->getInternal() != GL_INVALID_INDEX )
		{
			uint32_t index = 0u;

			for ( auto & resolveAttach : makeArrayView( subpass.pResolveAttachments, subpass.colorAttachmentCount ) )
			{
				if ( resolveAttach.attachment != VK_ATTACHMENT_UNUSED )
				{
					auto & srcAttach = get( frameBuffer )->getMsColourAttaches()[index];
					++index;
					auto & resolveAttaches = get( frameBuffer )->getResolveAttaches();
					auto resolveIt = std::find_if( resolveAttaches.begin()
						, resolveAttaches.end()
						, [&resolveAttach]( FboAttachment const & lookup )
						{
							return lookup.referenceIndex == resolveAttach.attachment;
						} );

					if ( resolveIt == resolveAttaches.end() )
					{
						reportError( frameBuffer
							, VK_ERROR_VALIDATION_FAILED_EXT
							, "Framebuffer Resolve"
							, "Couldn't find resolve attachment in fbo's resolve attachments." );
						continue;
					}

					auto & dstAttach = *resolveIt;

					if ( srcAttach.originalObject != dstAttach.originalObject
						|| srcAttach.originalMipLevel != dstAttach.originalMipLevel )
					{
						// Perform blit
						list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );
						srcAttach.bindRead( stack, srcAttach.mipLevel, GL_READ_FRAMEBUFFER, list );
						list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_DRAW_FRAMEBUFFER ) );
						dstAttach.bindDraw( stack, dstAttach.mipLevel, GL_DRAW_FRAMEBUFFER, list );
						list.push_back( makeCmd< OpType::eBlitFramebuffer >(
							0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
							0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
							GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST ) );
						list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
							, nullptr ) );
						list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_DRAW_FRAMEBUFFER
							, nullptr ) );
					}
				}
			}
		}
	}
}
