/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearColourFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildClearColourFboCommand( VkDevice device
		, ContextStateStack const & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list )
	{
		glLogCommand( list, "ClearColourFboCommand" );
		auto & glimage = *get( image );
		auto target = GL_TEXTURE_2D;

		if ( glimage.getSamples() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, get( device )->getBlitDstFbo() ) );
		auto point = getAttachmentPoint( glimage.getFormat() );

		for ( uint32_t level = 0u; level < glimage.getMipLevels(); ++level )
		{
			list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
				, point
				, target
				, get( image )->getInternal()
				, level ) );
			list.push_back( makeCmd< OpType::eDrawBuffers >( point ) );
			list.push_back( makeCmd< OpType::eClearColour >( value
				, 0u ) );
		}
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, stack.getCurrentFramebuffer() ) );
	}
}
