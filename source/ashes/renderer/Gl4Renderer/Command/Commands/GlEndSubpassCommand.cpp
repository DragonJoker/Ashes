/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndSubpassCommand.hpp"
#include "Command/Commands/GlBlitImageCommand.hpp"
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildEndSubpassCommand( VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list )
	{
		glLogCommand( "EndSubpassCommand" );

		if ( subpass.pResolveAttachments )
		{
			if ( get( frameBuffer )->getInternal() )
			{
				uint32_t index = 0u;

				for ( auto it = subpass.pResolveAttachments;
					it != subpass.pResolveAttachments + subpass.colorAttachmentCount;
					++it )
				{
					auto & resolveAttach = *it;
					auto & srcattach = get( frameBuffer )->getColourAttaches()[index++];
					auto & dstattach = get( frameBuffer )->getColourAttaches()[resolveAttach.attachment];

					if ( dstattach.object != GL_INVALID_INDEX )
					{
						list.push_back( makeCmd< OpType::eBindBuffer >( GL_DRAW_FRAMEBUFFER
							, get( frameBuffer )->getInternal() ) );
					}
					else
					{
						list.push_back( makeCmd< OpType::eBindBuffer >( GL_DRAW_FRAMEBUFFER
							, 0u ) );
					}

					list.push_back( makeCmd< OpType::eBindBuffer >( GL_READ_FRAMEBUFFER
						, get( frameBuffer )->getInternal() ) );
					list.push_back( makeCmd< OpType::eReadBuffer >( srcattach.point ) );
					UInt32Array attaches;
					attaches.push_back( dstattach.point );
					list.push_back( makeCmd< OpType::eDrawBuffers >( std::move( attaches ) ) );
					list.push_back( makeCmd< OpType::eBlitFramebuffer >( 
						0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
						0, 0, int32_t( get( frameBuffer )->getWidth() ), int32_t( get( frameBuffer )->getHeight() ),
						GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST ) );
				}
			}
		}
	}
}
