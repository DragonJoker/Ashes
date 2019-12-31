/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildClearDepthStencilFboCommand( VkDevice device
		, ContextStateStack const & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list )
	{
		glLogCommand( "ClearDepthStencilFboCommand" );
		auto & glimage = *get( image );
		auto target = GL_TEXTURE_2D;

		if ( glimage.getSamples() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, get( device )->getBlitDstFbo() ) );
		GLenum point = getAttachmentPoint( glimage.getFormat() );

		for ( uint32_t level = 0u; level < glimage.getMipLevels(); ++level )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, point
				, target
				, get( image )->getInternal()
				, level ) );
			auto format = get( image )->getFormat();

			if ( isDepthStencilFormat( format ) )
			{
				list.push_back( makeCmd< OpType::eClearDepthStencil >( value ) );
			}
			else if ( isDepthFormat( format ) )
			{
				list.push_back( makeCmd< OpType::eClearDepth >( value.depth ) );
			}
			else if ( isStencilFormat( format ) )
			{
				list.push_back( makeCmd< OpType::eClearStencil >( int32_t( value.stencil ) ) );
			}
		}

		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, stack.getCurrentFramebuffer() ) );
	}
}
