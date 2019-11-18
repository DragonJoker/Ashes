/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearAttachmentsCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl4_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
{
	void buildClearAttachmentsCommand( ContextStateStack & stack
		, VkClearAttachmentArray clearAttaches
		, VkClearRectArray clearRects
		, CmdList & list )
	{
		glLogCommand( "ClearAttachmentsCommand" );

		for ( auto & clearAttach : clearAttaches )
		{
			for ( auto & rect : clearRects )
			{
				VkScissorArray scissors
				{
					rect.rect
				};
				stack.apply( list, 0u, scissors, false );

				if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_COLOR_BIT ) )
				{
					auto & colour = clearAttach.clearValue.color;
					list.push_back( makeCmd< OpType::eClearColour >( colour
						, clearAttach.colorAttachment ) );
				}
				else
				{
					auto & depthStencil = clearAttach.clearValue.depthStencil;
					auto stencil = GLint( depthStencil.stencil );

					if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT ) )
					{
						list.push_back( makeCmd< OpType::eClearDepthStencil >( depthStencil ) );
					}
					else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT ) )
					{
						list.push_back( makeCmd< OpType::eClearDepth >( depthStencil.depth ) );
					}
					else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_STENCIL_BIT ) )
					{
						list.push_back( makeCmd< OpType::eClearStencil >( stencil ) );
					}
				}
			}
		}
	}
}
