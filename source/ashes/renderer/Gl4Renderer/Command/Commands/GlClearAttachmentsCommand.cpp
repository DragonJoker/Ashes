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
	namespace
	{
		void doClear( ContextLock const & context
			, VkClearAttachment const & clearAttach )
		{
			if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_COLOR_BIT ) )
			{
				auto & colour = clearAttach.clearValue.color;
				glLogCall( context
					, glClearBufferfv
					, GL_CLEAR_TARGET_COLOR
					, clearAttach.colorAttachment
					, colour.float32 );
			}
			else
			{
				auto & depthStencil = clearAttach.clearValue.depthStencil;
				auto stencil = GLint( depthStencil.stencil );

				if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT ) )
				{
					glLogCall( context
						, glClearBufferfi
						, GL_CLEAR_TARGET_DEPTH_STENCIL
						, 0u
						, depthStencil.depth
						, stencil );
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT ) )
				{
					glLogCall( context
						, glClearBufferfv
						, GL_CLEAR_TARGET_DEPTH
						, 0u
						, &depthStencil.depth );
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_STENCIL_BIT ) )
				{
					glLogCall( context
						, glClearBufferiv
						, GL_CLEAR_TARGET_STENCIL
						, 0u
						, &stencil );
				}
			}
		}

	}

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
				auto & scissor = rect.rect;

				if ( stack.getCurrentScissor() != scissor )
				{
					list.push_back( makeCmd< OpType::eApplyScissor >( scissor ) );
					stack.setCurrentScissor( scissor );
				}

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
