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
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		glLogCommand( list, "ClearColourFboCommand" );
		auto & glimage = *get( image );
		auto target = GL_TEXTURE_2D;

		if ( glimage.getSamples() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		bool hadFbo = stack.hasCurrentFramebuffer();
		list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_FRAMEBUFFER ) );
		auto point = getAttachmentPoint( glimage.getFormat() );

		for ( auto & range : ranges )
		{
			for ( auto level = range.baseMipLevel; level < range.baseMipLevel + range.levelCount; ++level )
			{
				if ( get( image )->getArrayLayers() > 1u )
				{
					for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
					{
						list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( GL_FRAMEBUFFER
							, point
							, get( image )->getInternal()
							, level
							, layer ) );
						list.push_back( makeCmd< OpType::eDrawBuffers >( point ) );
						list.push_back( makeCmd< OpType::eClearColour >( value
							, 0u ) );
					}
				}
				else
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
			}
		}

		if ( hadFbo )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, stack.getCurrentFramebuffer() ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );
		}
	}
}
